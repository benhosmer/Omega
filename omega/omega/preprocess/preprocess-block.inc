<?php
/**
 * @file
 * Contents of omega_preprocess_block().
 */

// adding a class to the title attributes
$vars['title_attributes_array']['class'] = 'block-title';
// give blocks placed through the menu_tree theme wrapper a specific class for proper theming
if (isset($vars['elements']['#theme_wrappers'][0]) && substr($vars['elements']['#theme_wrappers'][0], 0, 9) == 'menu_tree') {
  $vars['classes_array'][] = 'menu-block';
}
// add the menu-block class to some default blocks in Drupal for proper styling.
switch ($vars['block_html_id']) {
  case 'block-comment-recent':
    $vars['classes_array'][] = 'menu-block';
    break;
}
$block_data = $vars['elements']['#block'];
if(isset($block_data->subject) && strlen($block_data->subject) == 0) {
  // add a class to provide CSS for blocks without titles
  // in those cases, we want to chop out that lower border.
  $vars['classes_array'][] = 'block-without-title';
}

// add odd/even zebra classes into the array of classes
$vars['classes_array'][] = $vars['block_zebra'];
if($vars['block_id'] == 1) {
  $vars['classes_array'][] = 'first';
}
// The last_in_region property is set in omega_page_alter().
if (isset($vars['block']->last_in_region)) {
  $vars['classes_array'][] = 'last';
}
