<?php
/**
 * @file
 * Definition of NoteController Class
 */

class NoteController extends DrupalDefaultEntityController {

  public function save($note) {
    global $user;
    $note->note_uid = $user->uid;
    $note->note_timestamp = time();
    field_attach_presave('note', $note);
    if (!empty($note->note_id)) {
      drupal_write_record('note', $note, 'note_id');
      field_attach_update('note', $note);
      module_invoke_all('entity_update', 'note', $note);
      return $note;
    }
    drupal_write_record('note', $note);
    field_attach_insert('note', $note);
    module_invoke_all('entity_insert', 'note', $note);
    return $note;
  }
}

