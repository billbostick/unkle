<?php
// $Id: unkle.event.inc 47 2011-03-29 05:52:55Z bill $

/**
 * @file
 */


/**
 * Implements hook_menu().
 */
function unkle_event_menu() {
  // event menu items
  $items = array();
  $items['event/%unkle_event'] = array(
    'title callback' => 'unkle_event_page_title',
    'title arguments' => array(1),
    'page callback' => 'unkle_event_page',
    'page arguments' => array(1),
    'access arguments' => array('access unkle'),
    'file' => 'unkle.event.pages.inc',
  );
  $items['event/%unkle_event/view'] = array(
    'title' => 'View',
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );
  $items['event/%unkle_event/edit'] = array(
   'title' => 'Edit',
   'page callback' => 'drupal_get_form',
   'page arguments' => array('unkle_event_form_edit', 1),
   'access arguments' => array('access unkle'),
   'type' => MENU_LOCAL_TASK,
   'weight' => 10,
   'file' => 'unkle.event.pages.inc',
  );
  $items['admin/content/unkle/event'] = array(
    'title' => 'Events',
    'description' => 'Manage events on your site.',
    'access arguments' => array('administer unkle'),
    'page callback' => 'unkle_event_page_admin',
    'page arguments' => array('list'),
    'weight' => -10,
    'file' => 'unkle.event.pages.inc',
  );
  $items['admin/content/unkle/event/list'] = array(
    'title' => 'List',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'file' => 'unkle.event.pages.inc',
  );
  $items['admin/content/unkle/event/create'] = array(
    'title' => 'Add event',
    'page arguments' => array('create'),
    'access arguments' => array('administer unkle'),
    'type' => MENU_LOCAL_ACTION,
    'file' => 'unkle.event.pages.inc',
  );

  // experimenting with cstools modal dialogs
  $items['event/%unkle_event/edit/%ctools_js'] = array(
    'title' => 'Create Event',
    'page callback' => 'unkle_event_form_edit_ajax',
    'page arguments' => array(1, 3),
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
    'file' => 'unkle.event.pages.inc',
  );

  $items['event/%unkle_event/edit/%ctools_js/%'] = array(
    'title' => 'Post-login Action',
    'page callback' => 'unkle_event_form_ajax_success',
    'page arguments' => array(1, 3, 4),
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
    'file' => 'unkle.event.pages.inc',
  );

  return $items;
}

/*
function unkle_init() {

  // this is the way wrong place to put these, but it's making my life a lot easier for now.

  // Include the CTools tools that we need.
  ctools_include('ajax');
  ctools_include('modal');
  // Add CTools' javascript to the page.
  ctools_modal_add_js();
  drupal_add_js('misc/ajax.js');
}

//module_load_include('inc', 'unkle', 'unkle.events');
function unkle_get_person_events($person) {
  $events = array();
  foreach($person->field_events['und'] as $event) {
    $events[] = $event['value'];
  }
  return $events;
}

function unkle_get_events($events = array()) {
  $result = db_select('unkle_event', 'e')
              ->fields('e', array('event_id', 'event_type', 'title', 'date'))
              ->condition('event_id', $events, 'IN')
              ->execute();
  $output = '';
  foreach ($result as $event) {
    $output .= $event->event_type . ', ' .
               l($event->title, 'event/' . $event->event_id . '/edit') . ', '.
               $event->date .
               ctools_modal_text_button(t('edit'), 'event/' . $event->event_id . '/edit/nojs','Edit') . '<br />';
  }
  return $output;
}

function unkle_node_view($node, $view_mode, $landcode) {
  if ($node->type =='unkle_person') {
    $node->content['event_details'] = array(
      '#markup' => '<div id="events-container">' . 
                   unkle_get_events(unkle_get_person_events($node)) . 
                   '</div>',
      '#weight' => 10,
    );
  } 
  return $node;
}
 */

function unkle_get_person_events($person) {
  $events = array();
  foreach ($person->field_events['und'] as $event) {
    $events[] = $event['value'];
  }
  return $events;
}

function unkle_get_events($events = array()) {
  $result = db_select('unkle_event', 'e')
              ->fields('e', array('event_id', 'event_type', 'title', 'date'))
              ->condition('event_id', $events, 'IN')
              ->execute();
  $output = '';
  foreach ($result as $event) {
    $output .= $event->event_type . ', ' .
               l($event->title, 'event/' . $event->event_id . '/edit') . ', ' . $event->date .
               ctools_modal_text_button(t('edit'), 'event/' . $event->event_id . '/edit/nojs', 'Edit') . '<br />';
  }
  return $output;
}


function unkle_event_entity_info() {
  $info = array(
    'unkle_event' => array(
      'label' => t('Event'),
      'controller class' => 'EventController',
      'base table' => 'unkle_event',
      'uri callback' => 'unkle_event_uri',
      'entity keys' => array(
        'id' => 'event_id',
      ),
      'static_cache' => TRUE,
      'bundles' => array(
        'unkle_event' => array(
          'label' => t('Event'),
          'admin' => array(
            'path' => 'admin/content/unkle/event',
            'access arguments' => array('administer unkle'),
          ),
        ),
      ),
      'fieldable' => TRUE,
    ),
  );
  return $info;
}

function unkle_event_field_extra_fields() {
  $fields = array();

  $fields['unkle_event']['unkle_event'] = array(
    'form' => array(
      'event_type' => array(
        'label' => t('Type'),
        'description' => t('Unkle event type form element'),
        'weight' => -10,
        'widget' => array(
          'type' => 'text_textfield',
        ),
      ),
      'title' => array(
        'label' => t('Title'),
        'description' => t('Unkle event title form element'),
        'weight' => -10,
        'widget' => array(
          'type' => 'text_textfield',
        ),
      ),
      'date' => array(
        'label' => t('Date'),
        'description' => t('Unkle event date form element'),
        'weight' => -8,
        'widget' => array(
          'type' => 'date_text',
          'settings' => array(
            'input_format' => date_default_format('date_select'),
            'input_format_custom' => 'Y-m-d',
          ),
          'behaviors' => array(
            'multiple values' => FIELD_BEHAVIOR_CUSTOM,
            'default value' => FIELD_BEHAVIOR_CUSTOM,
          ),
        ),
      ),
    ),
  );
  return $fields;
}

function unkle_event_admin_paths() {
  $paths = array(
    'event/*/edit' => TRUE,
  );
  return $paths;
}

function unkle_event_uri($event) {
  return array(
    'path' => 'event/' . $event->event_id,
  );
}

function unkle_event_load($event_id, $reset = FALSE) {
  $events = unkle_event_load_multiple(array($event_id), array(), $reset);
  return reset($events);
}

function unkle_event_load_multiple($event_ids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('unkle_event', $event_ids, $conditions, $reset);
}

function unkle_event_page_title($event) {
  return $event->title;
}


