<?php

/**
 * @file
 * Contents of omega_preprocess_page().
 */
// if debug blocks are on, add in the javascript that will put those on the page for us.
if(omega_theme_get_setting('debug_block_toggle')) {
  //$regions = system_region_list($theme_key);
  //krumo($theme_info);
  $regions = $theme_info->info['regions'];
  $region_js = array();
  $exclude_regions = array(
    'page_top', 'page_bottom', 'user_bar_first', 'user_bar_second', 'dashboard_main', 'dashboard_inactive', 'dashboard_sidebar',
  );
  foreach ($regions AS $k => $v) {
    if(!in_array($k, $exclude_regions)) {
      $region_js[$k] = $v;
    }
  }
  $block_settings = array(
    'omega_debug_regions' => $region_js,
  );
  drupal_add_js($block_settings, 'setting');
}


$vars['menu_type'] = $menu_type = omega_theme_get_setting('omega_menu_type') ? omega_theme_get_setting('omega_menu_type') : 'omega';

// render the Omega Dropdowns. These are set to be phased out, but cannot be removed for sanity on existing sites.
if (variable_get('menu_main_links_source', 'main-menu') == variable_get('menu_secondary_links_source', 'user-menu') && $menu_type == 'omega') {
  $menu_depth = omega_theme_get_setting('omega_menu_depth') ? omega_theme_get_setting('omega_menu_depth') : 1;
  // render the menus as nested ULs to style up some drop downs.
  $vars['menu_data'] = $menu = omega_menu_structure(menu_tree_page_data(variable_get('menu_main_links_source', 'main-menu'), $menu_depth, FALSE));
  $vars['main_menu'] = theme('links', array(
    'links' => $menu,
    'attributes' => array(
      'class' => array('links', 'main-menu'),
    ),
    'heading' => array(
      'text' => t('Main menu'),
      'level' => 'h2',
      'class' => array('element-invisible'),
    )
  ));
  $vars['secondary_menu'] = FALSE;
}
// render default Drupal primary/secondary menus
elseif(variable_get('menu_main_links_source', 'main-menu') != variable_get('menu_secondary_links_source', 'user-menu') ||$menu_type == 'drupal') {
  // render default main & secondary menus (separate menus)
  if (isset($vars['main_menu'])) {
    $vars['main_menu'] = theme('links__system_main_menu', array(
      'links' => $vars['main_menu'],
      'attributes' => array(
        'class' => array('links', 'main-menu'),
      ),
      'heading' => array(
        'text' => t('Main menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      )
    ));
  }
  else {
    $vars['main_menu'] = FALSE;
  }
  if (isset($vars['secondary_menu'])) {
    $vars['secondary_menu'] = theme('links__system_secondary_menu', array(
      'links' => $vars['secondary_menu'],
      'attributes' => array(
        'class' => array('links', 'secondary-menu'),
      ),
      'heading' => array(
        'text' => t('Secondary menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      )
    ));
  }
  else {
    $vars['secondary_menu'] = FALSE;
  }
}

global $theme_key, $theme_info;
// theme_key is the name of the current theme
$vars['theme_key'] = $theme_key;
// theme_info is the array of theme information (region, settings, zones, etc.)
$vars['theme_info'] = $theme_info;
// default container width will be used in special zones and zones without a 
// container width defined in theme settings
$vars['default_container_width'] = omega_theme_get_setting('omega_default_container_width');
// pulling just the zone data out of the theme_info array
$theme_zones = $theme_info->info['zones'];
// creating empty array to hold our custom zone[group] data
$zones = array(
  'before' => array(),
  'content' => array(),
  'after' => array(),
);
// separate out the specific content zone (a very special case)
$content_zone = $theme_zones['content'];
// zone keys give us a way to find the numerical position of the content zone
// thus giving us a way to split up the array into before and after content zones
$zone_keys = array_keys($theme_zones);
// content_position is the numberical location of the content zone
$content_position = array_search('content', $zone_keys);
// zones_before_content are all zones that appear before content in the array
$zones_before_content = array_slice($theme_zones, 0, $content_position, TRUE);
// zones_after_content are all zones that appear after content in the array
$zones_after_content = array_slice($theme_zones, $content_position + 1, count($theme_zones), TRUE);

foreach ($theme_zones as $zone_ref => $regions) {
  
  $zone = array();
  //$zone['zone'] = array();
  $zone['zid'] = $zone_ref;
  $zone['enabled'] = omega_theme_get_setting('omega_'. $zone_ref .'_enabled') || omega_theme_get_setting('omega_'. $zone_ref .'_enabled') == 0 ? omega_theme_get_setting('omega_'. $zone_ref .'_enabled') : 1;
  $zone['wrapper'] = omega_theme_get_setting('omega_'. $zone_ref .'_wrapper') ? omega_theme_get_setting('omega_'. $zone_ref .'_wrapper') : 0;
  $zone['zone_type'] = omega_theme_get_setting('omega_'. $zone_ref .'_zone_type') ? omega_theme_get_setting('omega_'. $zone_ref .'_zone_type') : 'static';
  $zone['container_width'] = omega_theme_get_setting('omega_'. $zone_ref .'_container_width') ? omega_theme_get_setting('omega_'. $zone_ref .'_container_width') : $vars['default_container_width'];
  
  if ($zone['enabled']) {
    if(array_key_exists($zone_ref, $zones_before_content)) {
      $zone['type'] = 'before';
    }
    elseif(array_key_exists($zone_ref, $zones_after_content)) {
      $zone['type'] = 'after';
    }
    else {
      $zone['type'] = 'content';
    }
    $zone['#regions'] = array();
    foreach($regions as $region) {
      $zone['#regions'][$region] = isset($vars['page'][$region]) && is_array($vars['page'][$region]) ? $vars['page'][$region] : array();
      // we need to unset the region in the main $vars['page'] array now so that it isn't 'printed' before we get to it
      unset($vars['page'][$region]);
    }
    $zone['#theme'][] = 'zone';
    $zone['#zone'] = $zone['zid'];  
    
    $zone['#theme_hook_suggestions'][] = 'zone'; 
    $zone['#theme_hook_suggestions'][] = 'zone__' . $zone['type'];
    $zone['#theme_hook_suggestions'][] = 'zone__' . $zone['zid'];

    // pass page level data to each zone so that it could be implemented anywhere (title, tabs, etc.)
    $site_name = $vars['site_name'];
    $site_name_and_slogan = $vars['site_name'] . ' ' . $vars['site_slogan'];
    $vars['slogan'] = $vars['site_slogan'];
    if (isset($vars['logo'])) {
      $logo_img = $vars['logo_img'] = '<img src="' . $vars['logo'] . '" alt="' . $site_name_and_slogan . '" id="logo" />';
      $linked_logo_img = $vars['linked_logo_img'] = l($logo_img, '<front>', array('rel' => 'home', 'title' => t($vars['site_name']), 'html' => TRUE)); 
    }
    else {
      $logo_img = $vars['logo_img'] = FALSE;
      $linked_logo_img = $vars['linked_logo_img'] = FALSE;
    }
    if (isset($vars['site_name'])) {
      $linked_site_name = $vars['linked_site_name'] = l($vars['site_name'], '<front>', array('rel' => 'home', 'title' => t('Home')));
    }
    else {
      $linked_site_name = $vars['linked_site_name'] = FALSE;
    }
    $title = isset($vars['title']) ? $vars['title'] : drupal_get_title();
    $vars['title'] = $title;
    $vars['breadcrumb'] = theme('breadcrumb', array('breadcrumb' => drupal_get_breadcrumb()));
    $vars['menu_data'] = isset($vars['menu_data']) ? $vars['menu_data'] : FALSE;
    $zone['#omega'] = array(
      'page_data' => array(
        'content_attributes_array' => $vars['content_attributes_array'],
        'title' => $vars['title'],
        'title_prefix' => $vars['title_prefix'],
        'title_suffix' => $vars['title_suffix'],
        'title_attributes_array' => $vars['title_attributes_array'],
        'feed_icons' => $vars['feed_icons'],
        'logo' => $vars['logo'],
        'menu_type' => $vars['menu_type'],
        'main_menu' => $vars['main_menu'],
        'secondary_menu' => $vars['secondary_menu'],
        'site_name' => $vars['site_name'],
        'slogan' => $vars['slogan'],
        'logo_img' => $vars['logo_img'],
        'linked_logo_img' => $vars['linked_logo_img'],
        'linked_site_name' => $vars['linked_site_name'],
        'tabs' => $vars['tabs'],
        'menu_data' => $vars['menu_data'],
        'breadcrumb' => $vars['breadcrumb'],
        'default_container_width' => $vars['default_container_width'],
      ),
    );
    
    // assign the zone to the proper group of zones
    $zones[$zone['type']][$zone['zid']] = $zone;
  }
}

/**
  * Zone Assignments
  * 
  * Assign the zones to the page object to pass to page.tpl.php
  * The zones are grouped in zones_above, content_zone, and zones_below
  * This allows for dynamic region/zone placements on the fly via 
  * theme settings, the Delta module, and more without ever needing to 
  * edit page.tpl.php
  */
  
// zones appearing before content on page
$vars['page']['zones_above'] = array();
$before = array_keys($zones_before_content);
foreach($before as $k => $zone) {
  if (isset($zones['before'][$zone])) {
    $vars['page']['zones_above'][$zone] = $zones['before'][$zone];
  }
}

// required content zone
$vars['page']['content_zone'] = isset($zones['content']['content']) ? $zones['content']['content'] : FALSE;

// zones appearing after content on page
$vars['page']['zones_below'] = array();
$after = array_keys($zones_after_content);
foreach($after as $k => $zone) {
  if (isset($zones['after'][$zone])) {
    $vars['page']['zones_below'][$zone] = $zones['after'][$zone];
  }
}