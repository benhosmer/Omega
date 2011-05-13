<?php
// $Id$

function koi_form_system_theme_settings_alter(&$form, $form_state) {
  // Generate the form using Forms API. http://api.drupal.org/api/7
  $form['custom'] = array(
    '#title' => 'Custom theme settings', 
    '#type' => 'fieldset', 
  );
  $form['custom']['base_font_size'] = array(
    '#title' => 'Base font size',
    '#type' => 'select', 
    '#default_value' => theme_get_setting('base_font_size'),
    '#options' => array(
      '9px' => '9px',
      '10px' => '10px',
      '11px' => '11px',
      '12px' => '12px',
      '13px' => '13px',
      '14px' => '14px',
      '15px' => '15px',
      '16px' => '16px',
      '100%' => '100%',
    ),
  );
  $form['custom']['sidebar_first_weight'] = array(
    '#title' => 'First sidebar position', 
    '#type' => 'select',
    '#default_value' => theme_get_setting('sidebar_first_weight'),
    '#options' => array(
      -2 => 'Far left',
      -1 => 'Left',
       1 => 'Right',
       2 => 'Far right',
    ),
  );
  $form['custom']['sidebar_second_weight'] = array(
    '#title' => 'Second sidebar position', 
    '#type' => 'select',
    '#default_value' => theme_get_setting('sidebar_second_weight'),
    '#options' => array(
      -2 => 'Far left',
      -1 => 'Left',
       1 => 'Right',
       2 => 'Far right',
    ),
  );
  $form['custom']['layout_1'] = array(
    '#title' => '1-column layout', 
    '#type' => 'fieldset',
  );
  $form['custom']['layout_1']['layout_1_width'] = array(
    '#type' => 'select',
    '#title' => 'Width', 
    '#default_value' => theme_get_setting('layout_1_width'),
    '#options' => koi_generate_array(30, 100, 5, '%'),
  );
  $form['custom']['layout_1']['layout_1_min_width'] = array(
    '#type' => 'select',
    '#title' => 'Min width', 
    '#default_value' => theme_get_setting('layout_1_min_width'),
    '#options' => koi_generate_array(200, 1200, 10, 'px'),
  );
  $form['custom']['layout_1']['layout_1_max_width'] = array(
    '#type' => 'select',
    '#title' => 'Max width', 
    '#default_value' => theme_get_setting('layout_1_max_width'),
    '#options' => koi_generate_array(200, 1200, 10, 'px'),
  );
  $form['custom']['layout_2'] = array(
    '#title' => '2-column layout', 
    '#type' => 'fieldset',
  );
  $form['custom']['layout_2']['layout_2_width'] = array(
    '#type' => 'select',
    '#title' => 'Width', 
    '#default_value' => theme_get_setting('layout_2_width'),
    '#options' => koi_generate_array(30, 100, 5, '%'),
  );
  $form['custom']['layout_2']['layout_2_min_width'] = array(
    '#type' => 'select',
    '#title' => 'Min width', 
    '#default_value' => theme_get_setting('layout_2_min_width'),
    '#options' => koi_generate_array(200, 1200, 10, 'px'),
  );
  $form['custom']['layout_2']['layout_2_max_width'] = array(
    '#type' => 'select',
    '#title' => 'Max width', 
    '#default_value' => theme_get_setting('layout_2_max_width'),
    '#options' => koi_generate_array(200, 1200, 10, 'px'),
  );
  $form['custom']['layout_3'] = array(
    '#title' => '3-column layout', 
    '#type' => 'fieldset',
  );
  $form['custom']['layout_3']['layout_3_width'] = array(
    '#type' => 'select',
    '#title' => 'Width', 
    '#default_value' => theme_get_setting('layout_3_width'),
    '#options' => koi_generate_array(30, 100, 5, '%'),
  );
  $form['custom']['layout_3']['layout_3_min_width'] = array(
    '#type' => 'select',
    '#title' => 'Min width', 
    '#default_value' => theme_get_setting('layout_3_min_width'),
    '#options' => koi_generate_array(200, 1200, 10, 'px'),
  );
  $form['custom']['layout_3']['layout_3_max_width'] = array(
    '#type' => 'select',
    '#title' => 'Max width', 
    '#default_value' => theme_get_setting('layout_3_max_width'),
    '#options' => koi_generate_array(200, 1200, 10, 'px'),
  );
  $form['custom']['trim_pager'] = array(
    '#type' => 'select',
    '#title' => 'Trim pager after specified number of pages', 
    '#default_value' => theme_get_setting('trim_pager'),
    '#options' => koi_generate_array(4, 15, 1, ''),
  );
  $form['custom']['copyright_information'] = array(
    '#title' => 'Copyright information',
    '#description' => t('Information about copyright holder of the website - will show up at the bottom of the page'), 
    '#type' => 'textfield',
    '#default_value' => theme_get_setting('copyright_information'),
    '#size' => 60, 
    '#maxlength' => 128, 
    '#required' => FALSE,
  );
}

function koi_generate_array($min, $max, $increment, $postfix, $unlimited = NULL) {
  $array = array();
  if ($unlimited == 'first') {
    $array['none'] = 'Unlimited';
  }
  for ($a = $min; $a <= $max; $a += $increment) {
    $array[$a . $postfix] = $a . ' ' . $postfix;
  }
  if ($unlimited == 'last') {
    $array['none'] = 'Unlimited';
  }
  return $array;
}

