<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page while offline.
 *
 * All the available variables are mirrored in html.tpl.php and page.tpl.php.
 * Some may be blank but they are provided for consistency.
 *
 * @see template_preprocess()
 * @see template_preprocess_maintenance_page()
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>
<body class="<?php print $classes; ?>">
  <div id="page" class="container-16 clearfix">
    <div id="header" class="grid-16">
      <div id="logo-title">
        <div class="branding-data">
        <?php if (!empty($logo)): ?>
          
            <div class="logo-img">
            <a href="<?php print $base_path; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
              <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
            </a>
          </div>
        <?php endif; ?>
        </div>
        <div id="name-and-slogan" class="site-name-slogan">
          <h1 class="site-title"><a href="<?php print $base_path ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a></h1>
          <?php if(isset($site_slogan)): ?>
          <h6 class="site-slogan"><?php print $site_slogan; ?></h6>
          <?php endif; ?>
        </div> <!-- /name-and-slogan -->
      </div> <!-- /logo-title -->

      <?php if (!empty($header)): ?>
        <div id="header-region">
          <?php print $header; ?>
        </div>
      <?php endif; ?>

    </div> <!-- /header -->
    <div id="messages" class="grid-16">
      <?php if (!empty($messages)): print $messages; endif; ?>
    </div>
    
    <div id="container" class="grid-16 clearfix">
      <div id="content" class="maint">
        <?php if (!empty($title)): ?><h2 class="page-title" id="page-title"><?php print $title; ?></h2><?php endif; ?>
        <div id="content-content" class="clearfix">
          <?php print $content; ?>
        </div> <!-- /content-content -->
      </div> <!-- /content -->
    </div> <!-- /container -->
  </div> <!-- /page -->
</body>
</html>
