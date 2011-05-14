<?php

// fix messages 
// this should NOT be an empty string if it's freaking empty
if (isset($vars['messages']) && strlen($vars['messages']) == 0) {
  unset($vars['messages']);
}

// unset the action links array if it's empty
if (isset($vars['action_links']) && count($vars['action_links']) == 0) {
  unset($vars['action_links']);
}