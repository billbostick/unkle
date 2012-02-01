<?php

/**
 * @file
 * include file for the unkle sources module
 */

function unkle_sources_page($source, $view_mode = NULL) {
  // Remove previously built content, if exists.
  $source->content = array();

  // Build fields content.
  field_attach_prepare_view('source', array($source->source_id => $source), $view_mode);
  entity_prepare_view('source', array($source->source_id => $source));

  $source->content['source_details'] = array(
    '#markup' => '<pre>' . $source->source_pubfacts . '<br />' .
                 $source->source_text . '</pre>',
  );

  $source->content += field_attach_view('source', $source, $view_mode);
  return $source->content;
}

function _source_save(&$source) {
  return entity_get_controller('source')->save($source);
}

function unkle_sources_page_admin($tab = '') {
  switch ($tab) {
    case 'create':
      $build['unkle_sources_create'] = drupal_get_form('unkle_sources_form_edit');
      break;
    default:
      $build['unkle_sources_list'] = drupal_get_form('unkle_sources_form_list');
  }
  return $build;
}

function unkle_sources_form_list() {
  $header = array(
    'source_title' => array('data' => t('Title'), 'field' => 'ee.source_title'),
    'source_pubfacts' => array('data' => t('Publication Facts'), 'field' => 'ee.source_pubfacts'),
    'operations' => array('data' => t('Operations')),
  );
  $query = db_select('source', 'ee');
  $count_query = clone $query;
  $count_query->addExpression('COUNT(ee.source_id)');

  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('ee', array('source_id', 'source_title', 'source_pubfacts'))
    ->limit(20)
    ->orderByHeader($header)
    ->setCountQuery($count_query);
  $result = $query->execute();

  $destination = drupal_get_destination();

  $options = array();
  foreach ($result as $row) {
    $options[$row->source_id] = array(
      'source_title' => array('data' => array(
        '#type' => 'link',
        '#title' => $row->source_title,
        '#href' => "source/$row->source_id",
      )),
      'source_pubfacts' => $row->source_pubfacts,
      'operations' => array('data' => array(
        '#type' => 'link',
        '#title' => t('edit'),
        '#href' => "source/$row->source_id/edit",
        '#options' => array('query' => $destination),
      )),
    );
  }

  $form['source'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No entities available.'),
  );
  $form['pager']['#markup'] = theme('pager');

  return $form;
}

function unkle_sources_form_edit($form, &$form_state, $edit = NULL) {
  if (!isset($edit)) {
    $edit = (object) array(
      'source_title' => '',
      'source_pubfacts' => '',
      'source_text' => '',
    );
  }
  $form['source_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => 'The title of the work, record, or item and, when appropriate, the title of the larger work or series of which it is a part. For a published work, a book for example, might have a title plus the title of the series of which the book is a part. A magazine article would have a title plus the title of the magazine that published the article.',
    '#default_value' => $edit->source_title,
    '#required' => TRUE,
    '#weight' => 1,
  );
  $form['source_pubfacts'] = array(
    '#type' => 'textfield',
    '#title' => t('Publication Facts'),
    '#description' => 'When and where the record was created. For published works, this includes information such as the city of publication, name of the publisher, and year of publication.',
    '#default_value' => $edit->source_pubfacts,
    '#weight' => 2,
  );
  $form['source_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Source Text'),
    '#description' => "A verbatim copy of any description contained within the source. This indicates notes or text that are actually contained in the source document, not the submitter's opinion about the source. This should be, from the evidence point of view, \"what the original record keeper said\" as opposed to the researcher's interpretation.",
    '#default_value' => $edit->source_text,
    '#weight' => 3,
  );

  // Attach fields from Field module.
  field_attach_form('source', (object) $edit, $form, $form_state);

  // Store ID if any.
  if (!empty($edit->source_id)) {
    $form['source_id'] = array(
      '#type' => 'value',
      '#value' => $edit->source_id,
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

function unkle_sources_form_edit_validate($form, &$form_state) {
  // Attach validation from Field module.
  field_attach_form_validate('source', (object) $form_state['values'], $form, $form_state);
}

function unkle_sources_form_edit_submit($form, &$form_state) {
  $edit = (object) $form_state['values'];
  // Attach submit handlers from Field module.
  field_attach_submit('source', $edit, $form, $form_state);
  $source = _source_save($edit);
  $form_state['redirect'] = "source/$source->source_id";
}

// note-specific code

function unkle_notes_page($note, $view_mode = NULL) {
  // Remove previously built content, if exists.
  $note->content = array();

  // Build fields content.
  field_attach_prepare_view('note', array($note->note_id => $note), $view_mode);
  entity_prepare_view('note', array($note->note_id => $note));

  $note->content['note_body'] = array(
    '#markup' => '<pre>' . $note->note_body . '</pre>',
  );

  $note->content += field_attach_view('note', $note, $view_mode);
  return $note->content;
}

function _note_save(&$note) {
  return entity_get_controller('note')->save($note);
}

function unkle_notes_page_admin($tab = '') {
  switch ($tab) {
    case 'create':
      $build['unkle_notes_create'] = drupal_get_form('unkle_notes_form_edit');
      break;
    default:
      $build['unkle_notes_list'] = drupal_get_form('unkle_notes_form_list');
  }
  return $build;
}

function unkle_notes_form_list() {
  $header = array(
    'note_title' => array('data' => t('Title'), 'field' => 'ee.note_title'),
    'note_body' => array('data' => t('Body'), 'field' => 'ee.note_body'),
    'operations' => array('data' => t('Operations')),
  );
  $query = db_select('note', 'ee');
  $count_query = clone $query;
  $count_query->addExpression('COUNT(ee.note_id)');

  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('ee', array('note_id', 'note_title', 'note_body'))
    ->limit(20)
    ->orderByHeader($header)
    ->setCountQuery($count_query);
  $result = $query->execute();

  $destination = drupal_get_destination();

  $options = array();
  foreach ($result as $row) {
    $options[$row->note_id] = array(
      'note_title' => array('data' => array(
        '#type' => 'link',
        '#title' => $row->note_title,
        '#href' => "note/$row->note_id",
      )),
      'note_body' => $row->note_body,
      'operations' => array('data' => array(
        '#type' => 'link',
        '#title' => t('edit'),
        '#href' => "note/$row->note_id/edit",
        '#options' => array('query' => $destination),
      )),
    );
  }

  $form['notes'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No entities available.'),
  );
  $form['pager']['#markup'] = theme('pager');

  return $form;
}

function unkle_notes_form_edit($form, &$form_state, $edit = NULL) {
  if (!isset($edit)) {
    $edit = (object) array(
      'note_title' => '',
      'note_body' => '',
    );
  }
  $form['note_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $edit->note_title,
    '#required' => TRUE,
    '#weight' => 1,
  );
  $form['note_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => $edit->note_body,
    '#required' => TRUE,
    '#weight' => 2,
  );

  // Attach fields from Field module.
  field_attach_form('note', (object) $edit, $form, $form_state);

  // Store ID if any.
  if (!empty($edit->note_id)) {
    $form['note_id'] = array(
      '#type' => 'value',
      '#value' => $edit->note_id,
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

function unkle_notes_form_edit_validate($form, &$form_state) {
  // Attach validation from Field module.
  field_attach_form_validate('note', (object) $form_state['values'], $form, $form_state);
}

function unkle_notes_form_edit_submit($form, &$form_state) {
  $edit = (object) $form_state['values'];
  // Attach submit handlers from Field module.
  field_attach_submit('note', $edit, $form, $form_state);
  $note = _note_save($edit);
  $form_state['redirect'] = "note/$note->note_id";
}

// repo-specific code

function unkle_repos_page($repo, $view_mode = NULL) {
  // Remove previously built content, if exists.
  $repo->content = array();

  // Build fields content.
  field_attach_prepare_view('repository', array($repo->repo_id => $repo), $view_mode);
  entity_prepare_view('repository', array($repo->repo_id => $repo));

  $repo->content['repo_address'] = array(
    '#markup' => '<pre>' . $repo->repo_address . '<br />' .
                 $repo->repo_city . ', ' . $repo->repo_state .
                 ' ' . $repo->repo_postalcode . '<br />' .
                 $repo->repo_country . '</pre>',
  );

  $repo->content += field_attach_view('repository', $repo, $view_mode);
  return $repo->content;
}

function _repo_save(&$repo) {
  return entity_get_controller('repository')->save($repo);
}

function unkle_repos_page_admin($tab = '') {
  switch ($tab) {
    case 'create':
      $build['unkle_repos_create'] = drupal_get_form('unkle_repos_form_edit');
      break;
    default:
      $build['unkle_repos_list'] = drupal_get_form('unkle_repos_form_list');
  }
  return $build;
}

function unkle_repos_form_list() {
  $header = array(
    'repo_name' => array('data' => t('Name'), 'field' => 'ee.repo_name'),
    'repo_address' => array('data' => t('Address'), 'field' => 'ee.repo_address'),
    'repo_city' => array('data' => t('City'), 'field' => 'ee.repo_city'),
    'repo_state' => array('data' => t('State'), 'field' => 'ee.repo_state'),
    'operations' => array('data' => t('Operations')),
  );
  $query = db_select('repository', 'ee');
  $count_query = clone $query;
  $count_query->addExpression('COUNT(ee.repo_id)');

  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('ee', array('repo_id', 'repo_name', 'repo_address', 'repo_city', 'repo_state'))
    ->limit(20)
    ->orderByHeader($header)
    ->setCountQuery($count_query);
  $result = $query->execute();

  $destination = drupal_get_destination();

  $options = array();
  foreach ($result as $row) {
    $options[$row->repo_id] = array(
      'repo_name' => array('data' => array(
        '#type' => 'link',
        '#title' => $row->repo_name,
        '#href' => "repo/$row->repo_id",
      )),
      'repo_address' => $row->repo_address,
      'repo_city' => $row->repo_city,
      'repo_state' => $row->repo_state,
      'operations' => array('data' => array(
        '#type' => 'link',
        '#title' => t('edit'),
        '#href' => "repo/$row->repo_id/edit",
        '#options' => array('query' => $destination),
      )),
    );
  }

  $form['repository'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No entities available.'),
  );
  $form['pager']['#markup'] = theme('pager');

  return $form;
}

function unkle_repos_form_edit($form, &$form_state, $edit = NULL) {
  if (!isset($edit)) {
    $edit = (object) array(
      'repo_name' => '',
      'repo_address' => '',
      'repo_city' => '',
      'repo_state' => '',
      'repo_postalcode' => '',
      'repo_country' => '',
    );
  }
  $form['repo_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $edit->repo_name,
    '#description' => 'The official name of the archive in which the stated source material is stored.',
    '#required' => TRUE,
    '#weight' => 1,
  );
  $form['repo_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Address'),
    '#default_value' => $edit->repo_address,
    '#weight' => 2,
  );
  $form['repo_city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#default_value' => $edit->repo_city,
    '#weight' => 3,
  );
  $form['repo_state'] = array(
    '#type' => 'textfield',
    '#title' => t('State'),
    '#default_value' => $edit->repo_state,
    '#weight' => 4,
  );
  $form['repo_postalcode'] = array(
    '#type' => 'textfield',
    '#title' => t('Postal Code'),
    '#default_value' => $edit->repo_postalcode,
    '#weight' => 5,
  );
  $form['repo_country'] = array(
    '#type' => 'textfield',
    '#title' => t('Country'),
    '#default_value' => $edit->repo_country,
    '#weight' => 6,
  );

  // Attach fields from Field module.
  field_attach_form('repository', (object) $edit, $form, $form_state);

  // Store ID if any.
  if (!empty($edit->repo_id)) {
    $form['repo_id'] = array(
      '#type' => 'value',
      '#value' => $edit->repo_id,
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

function unkle_repos_form_edit_validate($form, &$form_state) {
  // Attach validation from Field module.
  field_attach_form_validate('repository', (object) $form_state['values'], $form, $form_state);
}

function unkle_repos_form_edit_submit($form, &$form_state) {
  $edit = (object) $form_state['values'];
  // Attach submit handlers from Field module.
  field_attach_submit('repository', $edit, $form, $form_state);
  $repo = _repo_save($edit);
  $form_state['redirect'] = "repo/$repo->repo_id";
}
