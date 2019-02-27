/* Variables */

variable "aws_access_key" {}
variable "aws_secret_key" {}

variable "cloudpose_ami_v2" {
  default = "ami-03cff97d967cc238a"
}

variable "subnet_id" {
  default = "subnet-1b716163"
}

variable "vpc_id" {
  default = "vpc-37bd555e"
}

variable "zone_id" {
  default = "Z38NKR99753R8C"
}

variable "domain_name" {
  default = "cloudpose.joelgaehwiler.com"
}

variable "key_name" {
  default = "personal"
}

variable "security_group" {
  default = "sg-156d987c"
}

/* Provider */

provider "aws" {
  region     = "eu-central-1"
  access_key = "${var.aws_access_key}"
  secret_key = "${var.aws_secret_key}"
}

/* Load Balancer */

resource "aws_lb" "cloudpose" {
  name               = "cloudpose"
  internal           = false
  load_balancer_type = "network"
  subnets            = ["${var.subnet_id}"]
}

resource "aws_lb_listener" "cloudpose" {
  load_balancer_arn = "${aws_lb.cloudpose.arn}"
  port              = 1337
  protocol          = "TCP"

  default_action {
    type             = "forward"
    target_group_arn = "${aws_lb_target_group.cloudpose.arn}"
  }
}

resource "aws_lb_target_group" "cloudpose" {
  name        = "cloudpose"
  protocol    = "TCP"
  port        = 1337
  vpc_id      = "${var.vpc_id}"
  target_type = "instance"

  deregistration_delay = 5

  health_check {
    protocol = "TCP"
    port     = 22
  }
}

resource "aws_route53_record" "cloudpose" {
  zone_id = "${var.zone_id}"
  name    = "${var.domain_name}"
  type    = "A"

  alias {
    name                   = "${aws_lb.cloudpose.dns_name}"
    zone_id                = "${aws_lb.cloudpose.zone_id}"
    evaluate_target_health = true
  }
}

/* Auto Scaling Group */

resource "aws_launch_configuration" "cloudpose-v2" {
  name            = "cloudpose-v2"
  image_id        = "${var.cloudpose_ami_v2}"
  instance_type   = "p2.xlarge"
  key_name        = "${var.key_name}"
  security_groups = ["${var.security_group}"]

  root_block_device {
    volume_type = "standard"
    volume_size = 16
  }
}

resource "aws_autoscaling_group" "cloudpose" {
  name             = "cloudpose"
  min_size         = 0
  max_size         = 4
  desired_capacity = 1

  launch_configuration = "${aws_launch_configuration.cloudpose-v2.name}"
  target_group_arns    = ["${aws_lb_target_group.cloudpose.arn}"]
  vpc_zone_identifier  = ["${var.subnet_id}"]

  wait_for_capacity_timeout = 0
}

resource "aws_autoscaling_schedule" "cloudpose-up" {
  scheduled_action_name  = "cloudpose-up"
  min_size               = 0
  max_size               = 4
  desired_capacity       = 1
  recurrence             = "0 8 * * 1-5"
  start_time             = "2019-02-28T08:00:00Z"
  autoscaling_group_name = "${aws_autoscaling_group.cloudpose.name}"
}

resource "aws_autoscaling_schedule" "cloudpose-down" {
  scheduled_action_name  = "cloudpose-down"
  min_size               = 0
  max_size               = 4
  desired_capacity       = 0
  recurrence             = "0 18 * * 1-5"
  start_time             = "2019-02-27T18:00:00Z"
  autoscaling_group_name = "${aws_autoscaling_group.cloudpose.name}"
}
