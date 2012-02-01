<?php
//$Id: unkle.event.pages.inc 46 2011-03-29 05:32:37Z bill $

/**
 * @file
 */

function unkle_event_form_edit_ajax($event, $js = NULL) {
  if (!$js) {
    return drupal_get_form('unkle_event_form_edit', $event);
  }
  ctools_include('modal');
  ctools_include('ajax');
  ctools_add_js('ajax-responder');
  $form_state = array('title' => t('Create Event'), 'ajax' => TRUE);
  $form_state['args'] = array($event);
  $form_state['build_info']['args'] = array($event);
  $output = ctools_modal_form_wrapper('unkle_event_form_edit', $form_state);

  if ($form_state['executed'] && $js) {
    $output = array(
      ctools_modal_command_dismiss(),
      ctools_ajax_command_reload(),
    );
  }
  print ajax_render($output);
  exit;
}
 
function unkle_event_form_edit_ajax_success($event, $js, $action) {
  if (!$js) {
    // we should never be here out of ajax context
    return MENU_NOT_FOUND;
  }
  ctools_include('ajax');
  ctools_add_js('ajax-responder');
  $commands = array();
//if ($action == 'inplace') {
    // stay here
    $commands[] = ctools_ajax_command_reload();
//}
//else {
    // bounce bounce
//  $commands[] = ctools_ajax_command_redirect('user');
//}
  print ajax_render($commands);
  exit;
}

function unkle_event_page($event, $view_mode = NULL) {
  $event->content = array();

  field_attach_prepare_view('unkle_event', array($event->event_id => $event), $view_mode);
  entity_prepare_view('event', array($event->event_id => $event));

  $event->content['event_details'] = array(
    '#markup' => '<pre>' . $event->event_type . '<br />' . $event->date . '</pre>',
  );

  $event->content += field_attach_view('event', $event, $view_mode);
  return $event->content;
}

function _unkle_event_save(&$event) {
  return entity_get_controller('unkle_event')->save($event);
}

function unkle_event_page_admin($tab = '') {
  switch ($tab) {
    case 'create':
      $build['unkle_event_create'] = drupal_get_form('unkle_event_form_edit');
      break;
    default:
      $build['unkle_event_list'] = drupal_get_form('unkle_event_form_list');
  }
  return $build;
}

function unkle_event_form_list() {
  $header = array(
    'title' => array('data' => t('Title'), 'field' => 'ee.title'),
    'event_type' => array('data' => t('Type'), 'field' => 'ee.event_type'),
    'date' => array('data' => t('Date'), 'field' => 'ee.date'),
    'operations' => array('data' => t('Operations')),
  );
  $query = db_select('unkle_event', 'ee');
  $count_query = clone $query;
  $count_query->addExpression('COUNT(ee.event_id)');

  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('ee', array('event_id', 'title', 'event_type', 'date'))
    ->limit(20)
    ->orderByHeader($header)
    ->setCountQuery($count_query);
  $result = $query->execute();

  $destination = drupal_get_destination();

  $options = array();
  foreach ($result as $row) {
    $options[$row->event_id] = array(
      'title' => array('data' => array(
        '#type' => 'link',
        '#title' => $row->title,
        '#href' => "event/$row->event_id",
      )),
      'event_type' => $row->event_type,
      'date' => $row->date,
      'operations' => array('data' => array(
        '#type' => 'link',
        '#title' => t('edit'),
        '#href' => "event/$row->event_id/edit",
        '#options' => array('query' => $destination),
      )),
    );
  }
  $form['unkle_event'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No entities available.'),
  );
  $form['pager']['#markup'] = theme('pager');

  return $form;
}

function unkle_event_form_edit($form, &$form_state, $edit = NULL) {
  if (!isset($edit)) {
    $edit = (object) array(
      'title' => '',
      'event_type' => '',
      'date' => '',
    );
  }
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $edit->title,
    '#required' => TRUE,
    '#weight' => 1,
  );
  $form['event_type'] = array(
    '#type' => 'textfield',
    '#title' => t('Type'),
    '#default_value' => $edit->event_type,
    '#required' => TRUE,
    '#weight' => 2,
  );
  $form['date'] = array(
    '#type' => 'textfield',
    '#title' => t('Date'),
    '#default_value' => $edit->date,
    '#required' => TRUE,
    '#weight' => 3,
  );

  // Attach fields from Field module.
  field_attach_form('unkle_event', (object) $edit, $form, $form_state);

  // Store ID if any.
  if (!empty($edit->event_id)) {
    $form['event_id'] = array(
      '#type' => 'value',
      '#value' => $edit->event_id,
    );
  }
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 5,
  );

  return $form;
}

function unkle_event_form_edit_validate($form, &$form_state) {
  // Attach validation from Field module.
  field_attach_form_validate('unkle_event', (object) $form_state['values'], $form, $form_state);
}

function unkle_event_form_edit_submit($form, &$form_state) {
  $edit = (object) $form_state['values'];
  // Attach submit handlers from Field module.
  field_attach_submit('unkle_event', $edit, $form, $form_state);
  $event = _unkle_event_save($edit);
  $form_state['redirect'] = "event/$event->event_id";
}
