<?php

/**
 * @file
 * Contents of omega_preprocess_region().
 */

// add a 'sidebar' class to the sidebar_first sidebar_second
switch($vars['region']) {
  case 'sidebar_first':
  case 'sidebar_second':  
    $vars['classes_array'][] = 'sidebar';
    break;
  
}

$omega_classes = isset($vars['elements']['#omega']['classes']) ? $vars['elements']['#omega']['classes'] : array();
$omega_attributes = isset($vars['elements']['#omega']['attributes']) ? $vars['elements']['#omega']['attributes'] : array();


$zone = isset($vars['elements']['#omega']['zid']) ? $vars['elements']['#omega']['zid'] : array();
$region = $vars['region'];
// set the extra classes to assign to this zone.
$region_classes = explode(' ', omega_theme_get_setting('omega_'. $zone . '_' . $region. '_region_extra_classes'));
foreach ($region_classes as $class) {
  if($class) {
    $vars['classes_array'][] = $class;
  }
}

$vars['classes_array'] = array_merge($vars['classes_array'], $omega_classes);
$vars['attributes_array'] = array_merge($vars['attributes_array'], $omega_attributes);

$vars['classes'] = implode(' ', $vars['classes_array']);
$vars['attributes'] = drupal_attributes($vars['attributes_array']);

// let's handle some special regions
if(isset($vars['elements']['#omega'])) {
  $cvars = $vars['elements']['#omega']['page_data'];
  foreach($cvars as $var => $val) {
    $vars[$var] = $val;
  }
}