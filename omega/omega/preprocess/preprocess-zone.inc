<?php

/**
 * @file
 * Contents of omega_preprocess_zone().
 * 
 * The functionality here is actually quite simple, applying to the container
 * regions of the 960 grid. 
 * 
 * There is not much processing needed to be done to the container elements
 * excluding checking for the appropriate classes/id to assign to the element
 */


/**
 * @file
 * Contents of omega_preprocess_zone().
 */

$zone = $vars['zone'];

// set the basic attributes for this zone
$vars['attributes_array'] = array(
  'class' => array(
    'clearfix', 
    'container-' . $zone['container_width'],
    'zone-' . $zone['zone_type'],
    'zone-' . $zone['type'],
    'zone',
  ),
  'id' => array($zone['zid'] . '-container'),
);

// set the extra classes to assign to this zone.
$zone_classes = explode(' ', omega_theme_get_setting('omega_'. $zone['zid'] . '_zone_extra_classes'));
foreach ($zone_classes as $class) {
  if(isset($class)) {
    $vars['attributes_array']['class'][] = $class;
  }
}
$vars['classes_array'] = $vars['attributes_array']['class'];
$vars['attributes'] = drupal_attributes($vars['attributes_array']);


$populated = FALSE;
$zone_regions = $zone['#regions'];
$block_debug = omega_theme_get_setting('debug_block_toggle', FALSE);
foreach($zone_regions AS $region => $region_data) {
  // let's account for a couple special regions
  switch($region) {
    case 'branding':
    case 'menu':
    case 'breadcrumb':
      // we have now made branding and menu REAL regions, but if they aren't defined in the .info file
      // for an existing subtheme, they must be dynamically created.
      // this will be skipped over for new subthemes
      $zone['#regions'][$region]['#theme_wrappers']= array('region');
      $zone['#regions'][$region]['#region'] = $region;
      $zone['#regions'][$region]['#theme_hook_suggestions'][] = 'region__' . $region;
      $zone['#regions'][$region]['#theme_hook_suggestions'][] = 'region'; 
    break;
  }
  // if we've turned on region debugging, let's create some dummy blocks. :)
  if($block_debug) {
    global $theme_info;
    $excluded_regions = array(
      'branding', 
      'menu', 
      'location', 
      'user_bar_first', 
      'user_bar_second', 
      'page_top', 
      'page_bottom', 
      'breadcrumb',
    );
    if (!in_array($region, $excluded_regions)) {
      $regions = $theme_info->info['regions'];
      $zone['#regions'][$region]['#theme_wrappers'] = array('region');
      $block = new stdClass();
      $block->delta = $region;
      $block->region = $region;
      $block->module = 'omega_tools';
      $block->subject = $regions[$region];
      $block->weight = -100;
      $zone['#regions'][$region]['omega_debug_'. $region] = array();
      
      $zone['#regions'][$region]['#region'] = $region;
      $zone['#regions'][$region]['#sorted'] = FALSE;
      $zone['#regions'][$region]['omega_debug_'. $region]['#printed'] = FALSE;
      $zone['#regions'][$region]['omega_debug_'. $region]['#theme_wrappers'] = array('block');
      $zone['#regions'][$region]['omega_debug_'. $region]['#block'] = $block;
      $zone['#regions'][$region]['omega_debug_'. $region]['#markup'] = '<div class="region-data">'.$region.'</div>';
    }
  }
  if(element_children($zone_regions[$region]) || $region == 'branding' || $region == 'menu' || $region == 'breadcrumb' || $block_debug) {
    $populated = TRUE;
  }
}














$zone_regions = $zone['#regions'];
$zone['populated'] = $populated;

$regions = array();
$region_order = array();
foreach($zone_regions AS $region => $region_data) {
  $region_width = omega_theme_get_setting('omega_' . $zone['zid'] . '_' . $region . '_region_width');
  $region_data = is_array($zone_regions[$region]) ? $zone_regions[$region] : FALSE;
  $regions[$region] = array(
    'data' => $region_data,
    'width' => $region_width,
    'spacing' => array(
      'prefix' => omega_theme_get_setting('omega_' . $zone['zid'] . '_' . $region . '_region_prefix'),
      'suffix' => omega_theme_get_setting('omega_' . $zone['zid'] . '_' . $region . '_region_suffix')
    ),
  );
  if($zone['zone_type'] == 'dynamic') {
    // dynamic regions take a bit more configuration to apply content first ordering
    $primary_region = omega_theme_get_setting('omega_'. $zone['zid'] .'_dynamic_zone_primary_region') == $region ? TRUE : FALSE;
    // let's put in the array of related zones.
    if ($primary_region) {
      $regions[$region]['primary'] = TRUE;
      $related_regions = array();
      foreach($zone_regions AS $r => $d) {
        // leave out the current region in related info
        if ($r != $region) {
          $related_regions[$r] = array(
            'width' => omega_theme_get_setting('omega_' . $zone['zid'] . '_' . $r . '_region_width'),
          );
        }
        $regions[$region]['related'] = $related_regions;
      }
    }
    $region_weight = intval(omega_theme_get_setting('omega_' . $zone['zid'] . '_' . $region . '_region_order'));
    $region_order[$region_weight] = $region;
  }
}

// run the appropriate region builder
// all this does is assign the appropriate 960.gs classes to the regions
switch($zone['zone_type']) {
  case 'static':
    $region_classes = static_region_builder($regions, $zone['container_width']);    
    break;
  case 'dynamic':
    $region_order = omega_order_reorder($region_order);
    $region_classes = dynamic_region_builder($regions, $zone['container_width'], $region_order);
    break;
}

// Set any region disabled by a size of zero, set it to look like it was already printed,
// making it not renderable again when drupal passes over the regions in the zone.
foreach($zone_regions AS $region => $region_data) {
  if(array_key_exists('hidden', $region_classes[$region])) {
    // edge case; unset regions that are defined as zero width or "hidden"
    $zone['#regions'][$region]['#printed'] = TRUE;
    //krumo($region . ' should be hidden...');
  }
}


// let's handle some special regions
if(isset($zone['#omega'])) {
  $cvars = $zone['#omega']['page_data'];
  foreach($cvars as $var => $val) {
    $vars[$var] = $val;
  }
}

$vars['menu_data'] = isset($vars['menu_data']) ? $vars['menu_data'] : FALSE;
// cycle regions again and assign custom data for later rendering
foreach($zone_regions AS $region => $region_data) {
  $special_regions = array('branding', 'menu', 'breadcrumb');
  if(element_children($zone['#regions'][$region]) || in_array($region, $special_regions)) { 
    // pass data along down the chain to the region 
    $zone['#regions'][$region]['#omega'] = array(
      'classes' => isset($region_classes[$region]) ? $region_classes[$region] : array(),
      'attributes' => array('id' => 'region-' . $region),
      'zid' => $zone['zid'],
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
  }
  else {
    unset($zone['#regions'][$region]);
  }
}

$vars['enabled'] = $zone['enabled'];
$vars['populated'] = $zone['populated'];
$vars['wrapper'] = $zone['wrapper'];
$vars['zid'] = $zone['zid'];
$vars['theme_hook_suggestions'] = $zone['#theme_hook_suggestions'];
// moving the region data out of $vars['zone'] so that it can be rendered as before in zone.tpl.php
$vars['regions'] = $zone['#regions'];