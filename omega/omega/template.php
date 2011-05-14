<?php

/*
##########################################################################################
      _                _                                  _                     _
   __| | _____   _____| | ___  _ __  _ __ ___   ___ _ __ | |_    __ _  ___  ___| | _____
  / _` |/ _ \ \ / / _ \ |/ _ \| '_ \| '_ ` _ \ / _ \ '_ \| __|  / _` |/ _ \/ _ \ |/ / __|
 | (_| |  __/\ V /  __/ | (_) | |_) | | | | | |  __/ | | | |_  | (_| |  __/  __/   <\__ \
  \__,_|\___| \_/ \___|_|\___/| .__/|_| |_| |_|\___|_| |_|\__|  \__, |\___|\___|_|\_\___/
                              |_|                               |___/
##########################################################################################
*/

/**
 * @file
 * Contains theme functions, preprocess and process overrides, and custom
 * functions for the Omega theme.
 */

// include general functions required both in template.php AND theme-settings.php
require_once(drupal_get_path('theme', 'omega') . '/inc/theme-functions.inc');
// include general theme override functions
require_once(drupal_get_path('theme', 'omega') . '/inc/theme.inc');

/**
  * Implements hook_preprocess().
  *
  * This function checks to see if a hook has a preprocess file associated with it
  * and if so, loads it.
  *
  * This makes it easier to keep sorted the preprocess functions that can be present
  * in the template.php file. You may still use hook_preprocess_page, etc in
  * template.php or create a file preprocess-page.inc in the preprocess folder to
  * include the appropriate logic to your preprocess functionality.
  *
  * @param $vars
  * @param $hook
  */
function omega_preprocess(&$vars, $hook) {
  // Collect all information for the active theme.
  $themes_active = array();
  global $theme_info;
  
  if (substr($hook, 0, 4) == 'zone') {
    $hook = 'zone';
  }
  // If there is a base theme, collect the names of all themes that may have
  // preprocess files to load.
  if (isset($theme_info->base_theme)) {
    global $base_theme_info;
    foreach ($base_theme_info as $base) {
      $themes_active[] = $base->name;
    }
  }

  // Add the active theme to the list of themes that may have preprocess files.
  $themes_active[] = $theme_info->name;
  // Check all active themes for preprocess files that will need to be loaded.
  foreach ($themes_active as $name) {
    if (is_file(drupal_get_path('theme', $name) . '/preprocess/preprocess-' . str_replace('_', '-', $hook) . '.inc')) {
      include(drupal_get_path('theme', $name) . '/preprocess/preprocess-' . str_replace('_', '-', $hook) . '.inc');
    }
  }
}

/**
 * Implementation of hook_process()
 * 
 * This function checks to see if a hook has a process file associated with 
 * it, and if so, loads it.
 * 
 * This makes it easier to keep sorted the process functions that can be present in the 
 * template.php file. You may still use hook_process_page, etc in template.php
 * or create a file process-page.inc in the process folder to include the appropriate
 * logic to your process functionality
 * 
 * @param $vars
 * @param $hook
 */
function omega_process(&$vars, $hook) {
// Collect all information for the active theme.
  $themes_active = array();
  global $theme_info;
  if (substr($hook, 0, 4) == 'zone') {
    $hook = 'zone';
  }
  // If there is a base theme, collect the names of all themes that may have 
  // preprocess files to load.
  if (isset($theme_info->base_theme)) {
    global $base_theme_info;
    foreach ($base_theme_info as $base) {
      $themes_active[] = $base->name;
    }
  }

  // Add the active theme to the list of themes that may have preprocess files.
  $themes_active[] = $theme_info->name;

  // Check all active themes for preprocess files that will need to be loaded.
  foreach ($themes_active as $name) {
    if (is_file(drupal_get_path('theme', $name) . '/process/process-' . str_replace('_', '-', $hook) . '.inc')) {
      include(drupal_get_path('theme', $name) . '/process/process-' . str_replace('_', '-', $hook) . '.inc');
    }
  }
}

/**
 * Implements template_preprocess_html().
 *
 * Preprocessor for page.tpl.php template file.
 * The default functionality can be found in preprocess/preprocess-page.inc
 */
function omega_preprocess_html(&$vars) {
  
}

/**
 * Implements template_preprocess_page().
 */
function omega_preprocess_page(&$vars) {

}

/**
 * Implements template_preprocess_node().
 */
function omega_preprocess_node(&$vars) {

}

/**
 * Implements template_process_page().
 */
function omega_process_page(&$vars) {

}

/**
 * Implements template_process_node().
 */
function omega_process_node(&$vars) {
  // Convert node attributes to a string and append to existing RDFa attributes.
  $vars['attributes'] .= drupal_attributes($vars['node_attributes']);
}


function omega_preprocess_zone(&$vars) {
  
}

function omega_process_zone(&$vars) {
  
}


/**
 * The rfilter function takes one argument, an array of values for the regions
 * for a "group" of regions like preface or postscript
 * @param $vars
 */
function rfilter($vars) {
  return count(array_filter($vars));
}


/**
 * Implements hook_css_alter().
 * Alter CSS files before they are output on the page.
 *
 * @param $css
 *   An array of all CSS items (files and inline CSS) being requested on the page.
 */
function omega_css_alter(&$css) {
  // fluid width option
  if (theme_get_setting('omega_fixed_fluid') == 'fluid') {
    $css_960 = drupal_get_path('theme', 'omega') . '/css/960.css';
    if (isset($css[$css_960])) {
      $css[$css_960]['data'] = drupal_get_path('theme', 'omega') . '/css/960-fluid.css';
    }
  }  
}

/**
 * Implements hook_theme().
 *
 * @todo figure out WTF with template suggestions
 * 
 * @see delta_theme()
 * @see http://api.drupal.org/api/function/hook_theme/7
 * - There was cause to create a module here to implement a proper theme 
 *   function. There was major issue with attempting to get the zone elements 
 *   to work properly. zone.tpl.php was being used when declared here in 
 *   omega_theme(), however, suggestions for more specific templates was NOT.
 *   
 * - The need here was to have the priority order be:
 *   - zone-ZONEID.tpl.php (the actual zone itself has a custom override)
 *     each have their own custom template to use for more generic implementations
 *   - zone.tpl.php (default)
 */
function omega_theme($existing, $type, $theme, $path) {
  $hooks = array();
  $preprocess_functions = array(
    'template_preprocess', 
    'template_preprocess_zone',
    'omega_preprocess',
    'omega_preprocess_zone',
  );
  $process_functions = array(
    'template_process', 
    'template_process_zone',
    'omega_process',
    'omega_process_zone'
  );
  $hooks['zone'] = array(
    'template' => 'zone',
    'path' => $path . '/templates',
    'render element' => 'zone',
    'pattern' => 'zone__',
    'preprocess functions' => $preprocess_functions,
    'process functions' => $process_functions,
  );
  return $hooks;
}


/**
 * Implements hook_page_alter
 */
function omega_page_alter($page) {
/**
 * Implements hook_page_alter().
 *
 * Look for the last block in the region. This is impossible to determine from
 * within a preprocess_block function.
 *
 * @param $page
 *   Nested array of renderable elements that make up the page.
 */
  // Look in each visible region for blocks.
  foreach (system_region_list($GLOBALS['theme'], REGIONS_VISIBLE) as $region => $name) {
    if (!empty($page[$region])) {
      // Find the last block in the region.
      $blocks = array_reverse(element_children($page[$region]));
      while ($blocks && !isset($page[$region][$blocks[0]]['#block'])) {
        array_shift($blocks);
      }
      if ($blocks) {
        $page[$region][$blocks[0]]['#block']->last_in_region = TRUE;
      }
    }
  }
}

function omega_form_alter(&$form, &$form_state, $form_id) {
  switch ($form_id) {
    // add a login link to the horizontal login bar block
    case 'user_login_block':
      if(omega_theme_get_setting('user_login_form')) {
        $form['links']['#markup'] = "";
        
        $items = array();
        $items[] = l(t('Login'), 'user/login', array('attributes' => array('title' => t('Log in.'), 'class' => 'login-submit-link')));
        if (variable_get('user_register', USER_REGISTER_VISITORS_ADMINISTRATIVE_APPROVAL)) {
          $items[] = l(t('Register'), 'user/register', array('attributes' => array('title' => t('Create a new user account.'))));
        }
        $items[] = l(t('Password'), 'user/password', array('attributes' => array('title' => t('Request new password via e-mail.'))));
        $form['links']['#markup'] = theme('item_list', array('items' => $items));
      }
    
      // HTML5 placeholder attribute
      $form['name']['#attributes']['placeholder'] = omega_theme_get_setting('user_login_name_placeholder');
      $form['pass']['#attributes']['placeholder'] = omega_theme_get_setting('user_login_pass_placeholder');

      break;
    case 'search_block_form':
      // HTML5 placeholder attribute
      $form['search_block_form']['#attributes']['placeholder'] = omega_theme_get_setting('omega_search_default_text');
      break;
  }
}

// hook_html_head_alter().
function omega_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8',
  );
}