<?php

/**
 * @file
 * Contents of omega_preprocess_comment().
 */
$vars['datetime'] = format_date($vars['comment']->created, 'custom', 'c');
$vars['unpublished'] = '';
if ($vars['status'] == 'comment-unpublished') {
  $vars['unpublished'] = '<div class="unpublished">' . t('Unpublished') . '</div>';
}