<?php
// $Id: unkle.admin.inc 42 2011-03-28 04:39:02Z bill $

/**
 * @file
 * Configuration pages for Unkle module.
 */

/**
 * Callback function for admin/config/content/unkle. Display the settings form.
 */
function unkle_settings($form, $form_state) {
  $form['unkle_familynum'] = array(
    '#type' => 'textfield',
    '#title' => 'Family Number',
    '#description' => 'The family number',
    '#default_value' => variable_get('unkle_familynum', ''),
  );

  return system_settings_form($form);
}

