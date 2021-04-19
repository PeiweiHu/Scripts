# Intro
A toy code to analyse firmware filesystem automatically, including web directory inference, bin/sbin md5 check, phpstan scan and firmwalker check. Result will be showed in pdf format.

# Requirements
First cd into root directory of this repo, then do as following.

## 1. Install phpstan (make sure php 7.0+ is installed already)

**1.1 install composer**

1. php -r "copy('https://install.phpcomposer.com/installer', 'composer-setup.php');"
2. php composer-setup.php
3.  (for Chinese users) composer config -g repo.packagist composer https://mirrors.aliyun.com/composer/
4. mv composer.phar /usr/local/bin/composer

**1.2 install phpstan**

1. sudo apt install php-mbstring
2. composer require --dev phpstan/phpstan

## 2. Install firmwalker

1. git clone https://github.com/craigz28/firmwalker.git
2. cd into firmwalker and run 'shodan init <API_KEY>'

## 3. Install pdfkit

1. sudo apt install wkhtmltopdf
2. pip3 install pdfkit