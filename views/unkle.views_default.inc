<?php

/**
 * @file
 * views integration for the unkle module
 */

/**
 * Implement hook_views_default_views().
 */
function unkle_views_default_views() {

$view = new view;
$view->name = 'unkle_people';
$view->description = '';
$view->tag = '';
$view->base_table = 'node';
$view->human_name = 'Unkle People';
$view->core = 7;
$view->api_version = '3.0-alpha1';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['row_plugin'] = 'fields';
/* Field: Node: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = 'Name';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['element_label_colon'] = 0;
$handler->display->display_options['fields']['title']['element_default_classes'] = 1;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: Fields: person_birthdate */
$handler->display->display_options['fields']['entity_id']['id'] = 'entity_id';
$handler->display->display_options['fields']['entity_id']['table'] = 'field_data_person_birthdate';
$handler->display->display_options['fields']['entity_id']['field'] = 'entity_id';
$handler->display->display_options['fields']['entity_id']['label'] = 'Birth date';
$handler->display->display_options['fields']['entity_id']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['trim'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['entity_id']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['entity_id']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['html'] = 0;
$handler->display->display_options['fields']['entity_id']['element_label_colon'] = 1;
$handler->display->display_options['fields']['entity_id']['element_default_classes'] = 1;
$handler->display->display_options['fields']['entity_id']['hide_empty'] = 0;
$handler->display->display_options['fields']['entity_id']['empty_zero'] = 0;
$handler->display->display_options['fields']['entity_id']['settings'] = array(
  'format_type' => 'long',
  'fromto' => 'both',
  'multiple_number' => '',
  'multiple_from' => '',
  'multiple_to' => '',
  'show_repeat_rule' => 'show',
);
/* Field: Fields: person_deathdate */
$handler->display->display_options['fields']['entity_id_1']['id'] = 'entity_id_1';
$handler->display->display_options['fields']['entity_id_1']['table'] = 'field_data_person_deathdate';
$handler->display->display_options['fields']['entity_id_1']['field'] = 'entity_id';
$handler->display->display_options['fields']['entity_id_1']['label'] = 'Death date';
$handler->display->display_options['fields']['entity_id_1']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['entity_id_1']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['entity_id_1']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['entity_id_1']['alter']['trim'] = 0;
$handler->display->display_options['fields']['entity_id_1']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['entity_id_1']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['entity_id_1']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['entity_id_1']['alter']['html'] = 0;
$handler->display->display_options['fields']['entity_id_1']['element_label_colon'] = 1;
$handler->display->display_options['fields']['entity_id_1']['element_default_classes'] = 1;
$handler->display->display_options['fields']['entity_id_1']['hide_empty'] = 0;
$handler->display->display_options['fields']['entity_id_1']['empty_zero'] = 0;
$handler->display->display_options['fields']['entity_id_1']['settings'] = array(
  'format_type' => 'long',
  'fromto' => 'both',
  'multiple_number' => '',
  'multiple_from' => '',
  'multiple_to' => '',
  'show_repeat_rule' => 'show',
);
/* Filter: Node: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'unkle_person' => 'unkle_person',
);

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'person_list');
$handler->display->display_options['defaults']['pager'] = FALSE;
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '50';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'entity_id' => 'entity_id',
  'entity_id_1' => 'entity_id_1',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'entity_id' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'entity_id_1' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['path'] = 'people';
$handler->display->display_options['menu']['type'] = 'normal';
$handler->display->display_options['menu']['title'] = 'People';
$handler->display->display_options['menu']['weight'] = '1';
$translatables['unkle_people'] = array(
  t('Defaults'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort By'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('Name'),
  t('Birth date'),
  t('Death date'),
  t('Page'),
);

$views[$view->name] = $view;
return $views;
}

