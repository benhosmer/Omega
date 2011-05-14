<?php
$themes = list_themes();
$theme = $form_state['build_info']['args'][0];
$settings_theme_info = $themes[$theme]->info;

for ($i = 0; $i <= 32; $i++) {
  $grids[$i] = $i;
}
for ($i = 0; $i <= 31; $i++) {
  $spacing[$i] = $i;
}
$containers = array(
  '12' => '12 column grid (940 usable pixels)',
  '16' => '16 column grid (940 usable pixels)',
  '24' => '24 column grid (950 usable pixels)',
  '32' => '32 column grid (950 usable pixels)',
);
// Region Settings

// create a wrapper fieldset for default grid configurations
$form['omega_grid']['defaults'] = array(
  '#type' => 'fieldset',
  '#title' => t('960gs Default Config'),
  '#weight' => -100,
);
// provide default container width
$form['omega_grid']['defaults']['omega_default_container_width'] = array(
  '#type' => 'select',
  '#title' => t('Default container width'),
  '#default_value' => omega_theme_get_setting('omega_default_container_width'),
  '#options' => $containers,
  '#weight' => -50,
  '#description' => t('This width is used for regions like $help, $messages and other non-important regions in page.tpl.php'),
);
// select fixed/fluid for all zones/entire site.
$default_omega_layout = omega_theme_get_setting('omega_fixed_fluid') ? omega_theme_get_setting('omega_fixed_fluid') : 'fixed';
$form['omega_grid']['defaults']['omega_fixed_fluid'] = array(
  '#type' => 'radios',
  '#description' => t('You may select fluid layout, or the default fixed width layout.'),
  '#title' => t('Fixed / Fluid Layout'),
  '#default_value' => $default_omega_layout,
  '#options' => array(
    'fixed' => t('Fixed width (theme default)'),
    'fluid' => t('Fluid width'),
  ),
);

$zones = $settings_theme_info['zones'];
$all_regions = $settings_theme_info['regions'];
// Looping over each zone, and region in that zone.
foreach ($zones AS $zone => $regions) {
  // creating an additional fieldset for each zone 
  $form['omega_grid'][$zone] = array(
    '#type' => 'fieldset',
    '#title' => t(ucwords($zone) . ' Zone Config'),
    //'#prefix' => t('<h3 class="omega-accordion"><a href="#">' . ucwords($zone) . ' Zone Config</a></h3><div class="omega-accordion-content">'),
    //'#suffix' => '</div>', 
  );
  // provide a quick enable/disable option for this zone
  $zone_enabled = omega_theme_get_setting('omega_'. $zone .'_enabled') || omega_theme_get_setting('omega_'. $zone .'_enabled') == 0 ? omega_theme_get_setting('omega_'. $zone .'_enabled') : 1;
  $form['omega_grid'][$zone]['omega_'. $zone .'_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable this Zone.'),
    '#description' => t('Disabling this zone will instantly hide the container and all regions within from being rendered on page.'),
    '#default_value' => $zone_enabled,
  '#weight' => -50,
  );
  // provide a full width wrapper around the zone (allowing for design elements outside the grid)
  $zone_wrapper = omega_theme_get_setting('omega_'. $zone .'_wrapper') ? omega_theme_get_setting('omega_'. $zone .'_wrapper') : 0;
  $form['omega_grid'][$zone]['omega_'. $zone .'_wrapper'] = array(
    '#type' => 'checkbox',
    '#title' => t('Provide full width wrapper around this zone.'),
    '#description' => t('Enabling this feature will give a &lt;div&gt; wrapper around the zone itself, allowing you to theme in elements that appear outside the 960 pixel container zone.'),
    '#default_value' => $zone_wrapper,
  );
  // create a container width selection menu for EACH zone
  $form['omega_grid'][$zone]['omega_'. $zone .'_container_width'] = array(
    '#type' => 'select',
    '#title' => t('Container width'),
    '#default_value' => omega_theme_get_setting('omega_'. $zone .'_container_width'),
    '#options' => $containers,
  );
  // create a chooser for static/dynamic zone
  $zone_type = omega_theme_get_setting('omega_'. $zone .'_zone_type') ? omega_theme_get_setting('omega_'. $zone .'_zone_type') : 'static';
  $form['omega_grid'][$zone]['omega_'. $zone .'_zone_type'] = array(
    '#type' => 'radios',
    '#description' => t('<p><strong>Static</strong> Zones are displayed either inline or stacked based on settings.</p><p><strong>Dynamic</strong> Zones are always displayed inline, with a primary region and MUST fit inside the container wrap. Dynamic zones also allow for push & pull classes for content first ordering.'),
    '#title' => t('Zone Type'),
    '#default_value' => $zone_type,
    '#options' => array(
      'static' => t('Static Zone'),
      'dynamic' => t('Dynamic Zone'),
    ),
  );
  // provide content first layout ordering via push/pull if this is a dynamic zone
  $region_options = array();
  // must change to a for loop to then foreach over the "associated items"
  $i=0;
  foreach($regions as $region) {
    $option_val = $region;
    if (!isset($all_regions[$region])) {
      $option_txt = ucwords($region);
    }
    else {
      $option_txt = $all_regions[$region];
    }
    $region_options[$region] = $option_txt;
  }
  // decide which region is the primary item
  // the "primary" region is the one that will absorb the size of empty regions
  // that are related in the same zone.
  $form['omega_grid'][$zone]['omega_'. $zone .'_dynamic_zone_primary_region'] = array(
    '#type' => 'select',
    '#title' => t('Primary Region'),
    '#default_value' => omega_theme_get_setting('omega_'. $zone .'_dynamic_zone_primary_region'),
    '#options' => $region_options,
    '#description' => t('The primary region is the one that will gain width when associated regions are empty.'),
    '#states' => array(
      'visible' => array(
        ':input[name="omega_'. $zone .'_zone_type"]' => array('value' => t('dynamic')),
      ),
    ),
  );
  // allow for zone classes
  $form['omega_grid'][$zone]['omega_' . $zone . '_zone_extra_classes'] = array(
    '#type' => 'textfield',
    '#title' => t('Additional Zone Classes'),
    '#size' => 60,
    '#default_value' => omega_theme_get_setting('omega_' . $zone . '_zone_extra_classes'),
    '#description' => t('You can quickly add custom CSS classes to your zone here.'),
  );
  // create friendly names for regions
  // "normal" regions provide these automatically
  // "special" regions (branding, menu, breadcrumb, search need to have a 
  // friendly name created on the fly with ucwords()
  foreach($regions as $region) {
    if (!isset($all_regions[$region]) || $region == 'content') {
      $locked_flag = ' (special region)';
      $region_name = ucwords(str_replace('_', ' ', $region));
    }
    else {
      $locked_flag = '';
      $region_name = $all_regions[$region];
    }
    // switch this area to place these region groups via weight of the region
    $weight = intval(omega_theme_get_setting('omega_' . $zone . '_' . $region . '_region_weight') + 100);
    // create a fieldset around the regions so they can be weighted by the order
    // if they are a dynamic region.
    $form['omega_grid'][$zone][$region] = array(
      '#type' => 'fieldset', 
    );
    // only add weight value to dynamic zones
    if($zone_type == 'dynamic') {
      $form['omega_grid'][$zone][$region]['#weight'] = $weight;
    }
    // create a prefix dropdown
    $form['omega_grid'][$zone][$region]['omega_' . $zone . '_' . $region . '_region_prefix'] = array(
      '#prefix' => t('<h4 class="region-title">' . $region_name . $locked_flag . '</h4><div class="prefix_config">'),
      '#type' => 'select',
      '#title' => t('Prefix'),
      '#default_value' => omega_theme_get_setting('omega_' . $zone . '_' . $region . '_region_prefix'),
      '#options' => $spacing,
      '#suffix' => '</div>',
    );
    // create the width dropdown
    $form['omega_grid'][$zone][$region]['omega_' . $zone . '_' . $region . '_region_width'] = array(
      '#prefix' => '<div class="width_config">',
      '#type' => 'select',
      '#title' => t('Width'),
      '#default_value' => omega_theme_get_setting('omega_' . $zone . '_' . $region . '_region_width'),
      '#options' => $grids,
      '#suffix' => '</div>',
    );
    // create a suffix dropdown
    $form['omega_grid'][$zone][$region]['omega_' . $zone . '_' . $region . '_region_suffix'] = array(
      '#prefix' => '<div class="suffix_config">',
      '#type' => 'select',
      '#title' => t('Suffix'),
      '#default_value' => omega_theme_get_setting('omega_' . $zone . '_' . $region . '_region_suffix'),
      '#options' => $spacing,
      '#suffix' => '</div>',
    );
    // build a weight/order select based on the number of regions in the zone
    $weights = array();
    for ($i = 1; $i <= count($regions); $i++) {
      $weights[$i] = $i;
    }
    // allow for ordering the regions in dyanmic zones
    $form['omega_grid'][$zone][$region]['omega_' . $zone . '_' . $region . '_region_order'] = array(
      '#prefix' => '<div class="weight_config">',
      '#type' => 'select',
      '#title' => t('Order'),
      '#default_value' => omega_theme_get_setting('omega_' . $zone . '_' . $region . '_region_order'),
      '#options' => $weights,
      //'#description' => t('Only used for dynamic zones, the weight will determine in what order the regions are rendered in page.tpl.php, and also on this form to demonstrate the order in which they will appear. However, source ordering will remain intact as defined in the zones[] array in your .info file.'),
      '#suffix' => '</div>',
      '#states' => array(
        'visible' => array(
          ':input[name="omega_'. $zone .'_zone_type"]' => array('value' => t('dynamic')),
        ),
      ),
      
    );
    // allow adding of classes for regions
    $form['omega_grid'][$zone][$region]['omega_' . $zone . '_' . $region . '_region_extra_classes'] = array(
      '#prefix' => '<div class="classes_config">',
      '#type' => 'textfield',
      '#title' => t('Additional Region Classes'),
      '#size' => 60,
      '#default_value' => t(omega_theme_get_setting('omega_' . $zone . '_' . $region . '_region_extra_classes')),
      '#description' => t('You can quickly add custom CSS classes to your regions here.'),
      '#suffix' => '</div>',
    );
  }
}