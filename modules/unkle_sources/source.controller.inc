<?php
/**
 * @file
 * Definition of SourceController Class
 */
class SourceController extends DrupalDefaultEntityController {

  public function save($source) {
    global $user;
    $source->source_uid = $user->uid;
    $source->source_timestamp = time();
    field_attach_presave('source', $source);
    if (!empty($source->source_id)) {
      drupal_write_record('source', $source, 'source_id');
      field_attach_update('source', $source);
      module_invoke_all('entity_update', 'source', $source);
      return $source;
    }
    drupal_write_record('source', $source);
    field_attach_insert('source', $source);
    module_invoke_all('entity_insert', 'source', $source);
    return $source;
  }
}

