<?php
// $Id: unkle.gedcom.inc 53 2011-03-31 23:36:02Z bill $

/**
 * @file
 * GEDCOM import and export for Unkle
 */

/**
 * Implements hook_menu().
 */

$gedcom = array();

function unkle_gedcom_menu() {
  $items['gedcom_import_results'] = array(
    'title' => 'Import GEDCOM',
    'page callback' => 'gedcom_import_results',
    'access arguments' => array('access content'),
    'type' => MENU_CALLBACK,
    'file' => 'unkle.gedcom.inc',
  );
  $items['admin/content/unkle/import'] = array(
    'title' => 'Import GEDCOM',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('unkle_gedcom_import_form'),
    'access arguments' => array('access content'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'unkle.gedcom.inc',
    'weight' => 10,
  );
  $items['admin/content/unkle/pdelete'] = array(
    'title' => 'Delete Unkle Data',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('unkle_gedcom_delete_persons_form'),
    'access arguments' => array('access content'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'unkle.gedcom.inc',
    'weight' => 11,
  );

  return $items;
}

function gedcom_import_results() {

  include("includes/gedcom.import.inc");
  $out = '';
  if ($cache = cache_get('unkle_gedcom_file', 'cache')) {
  
    $file = unserialize($cache->data);
    $filename = file_unmanaged_copy($file->uri);
    $gedcom = gedcom_parse_file($filename);
    $message = t('Import from GEDCOM:') . $filename . '<br />' .
                          'People Imported:' . $gedcom['counts']['indi'] . '<br />' .
                          'Families Imported:' . $gedcom['counts']['fam'] . '<br />' .
                          'Notes Imported:' . $gedcom['counts']['note'] . '<br />' .
                          'Media Objects Imported:' . $gedcom['counts']['obje'] . '<br />' .
                          'Repositories Imported:' . $gedcom['counts']['repo'] . '<br />' .
                          'Sources Imported:' . $gedcom['counts']['sour'];
    drupal_set_message(check_plain($message));


    // create person nodes from $gedcom['indi'] array
    if (is_array($gedcom['indi']) && count($gedcom['indi'])) {
      foreach ($gedcom['indi'] as $key => $indi) {
        if ($nid = unkle_gedcom_new_person($indi)) {
          // add a node id field to each element of $gedcom['indi']
          $gedcom['indi'][$key]['node_id'] = $nid;
        }
      }
    }
    

    // add gedcom data
//  if (is_array($gedcom['indi']) && count($gedcom['indi'])) {
//    foreach ($gedcom['indi'] as $key => $indi) {
//      if (key_exists('node_id', $indi) && $indi['node_id']) {
//        unkle_add_gedcom($indi['node_id'], $indi['gedcom']); 
//      }
//    }
//  }

    // create family nodes from $gedcom['fam'] array
    if (is_array($gedcom['fam']) && count($gedcom['fam'])) {
      foreach ($gedcom['fam'] as $key => $fam) {
        if ($nid = unkle_gedcom_new_family($fam)) {
          // add a node id field to each element of $gedcom['fam']
          $gedcom['fam'][$key]['node_id'] = $nid;
        }
      }
    }

    if (is_array($gedcom['indi']) && count($gedcom['indi'])) {
      foreach ($gedcom['indi'] as $key => $indi) {
        // patch family node id into person node FAMS field
        if (array_key_exists('fams', $indi) && is_array($indi['fams'])) {
          foreach ($indi['fams'] as $famsrec) {            
            $fam_nid = 0;
            foreach ($gedcom['fam'] as $fam) {
              if ($fam['ref'] == $famsrec['ref']) {
                unkle_add_fams($indi['node_id'], $fam['node_id']);
              }
            }
          }
        }
        // patch family node id into person node FAMC field
        if (array_key_exists('famc', $indi) && is_array($indi['famc'])) {
          foreach ($indi['famc'] as $famcrec) {            
            $fam_nid = 0;
            foreach ($gedcom['fam'] as $fam) {
              if ($fam['ref'] == $famcrec['ref']) {
                unkle_add_famc($indi['node_id'], $fam['node_id']);
              }
            }
          }
        }
      }
    }

    if (is_array($gedcom['fam']) && count($gedcom['fam'])) {
      foreach ($gedcom['fam'] as $key => $fam) {
        // patch person node id into family node HUSB field
        if (array_key_exists('husb', $fam)) {
          $indi_nid = 0;
          foreach ($gedcom['indi'] as $indi) {
            if ($indi['ref'] == $fam['husb']) {
              unkle_add_husb($fam['node_id'], $indi['node_id']);
            }
          }
        }
        // patch person node id into family node WIFE field
        if (array_key_exists('wife', $fam)) {
          $indi_nid = 0;
          foreach ($gedcom['indi'] as $indi) {
            if ($indi['ref'] == $fam['wife']) {
              unkle_add_wife($fam['node_id'], $indi['node_id']);
            }
          }
        }
        // patch person node id into family node CHIL field
        if (array_key_exists('chil', $fam) && is_array($fam['chil'])) {
          foreach ($fam['chil'] as $chilref) {
            $indi_nid = 0;
            foreach ($gedcom['indi'] as $indi) {
              if ($indi['ref'] == $chilref) {
                unkle_add_child($fam['node_id'], $indi['node_id']);
              }
            }
          }
        }
        // patch up family node title
        unkle_fam_title($fam['node_id']);
      }
    }
//  dpm($gedcom);
  }
  $out .= '<br />Import Complete!<br />';
  return $out;
}

function unkle_fam_title($family_nid) {
  $family = node_load($family_nid);
  $family->title = unkle_family_title($family);
  node_save($family);
}

function unkle_add_husb($family_nid, $person_nid) {
  $family = node_load($family_nid);
  $family->family_husband['und'][]['nid'] = $person_nid;
  node_save($family);
}

function unkle_add_wife($family_nid, $person_nid) {
  $family = node_load($family_nid);
  $family->family_wife['und'][]['nid'] = $person_nid;
  node_save($family);
}

function unkle_add_gedcom($node_id, $gedcom) {
  $node = node_load($node_id);
  $gedcom_text = '';
  foreach ($gedcom as $next_gedcom_line) {
    if (strlen($gedcom_text) <= 16384) {
      $gedcom_text .= $next_gedcom_line;
    }
  }
  $node->person_gedcom[$node->language][0]['value'] = substr($gedcom_text, 0, 16384);
  node_save($node);
}

function unkle_add_child($family_nid, $person_nid) {
  $family = node_load($family_nid);
  $family->family_child['und'][]['nid'] = $person_nid;
  node_save($family);
}

function unkle_add_fams($person_nid, $family_nid) {
  $person = node_load($person_nid);
  $person->person_spousetofamily['und'][]['nid'] = $family_nid;
  node_save($person);
}

function unkle_add_famc($person_nid, $family_nid) {
  $person = node_load($person_nid);
  $person->person_childtofamily['und'][]['nid'] = $family_nid;
  node_save($person);
}

function unkle_gedcom_new_person($indi) {
  $node = new stdClass();
  $node->type = 'unkle_person';
  $node->language = LANGUAGE_NONE;
  node_object_prepare($node);
  // parse birth and death dates
  $birthdate = '';
  $deathdate = '';
  if (array_key_exists('birt', $indi) && array_key_exists('date', $indi['birt'])) {
    $parts = date_parse($indi['birt']['date']);
    if ($parts['year'] && $parts['month'] && $parts['day']) {
      $birthdate = $parts['year'] . '-' . $parts['month'] . '-' . $parts['day'] . 'T08:00:00';
      $node->person_birthdate['und'][0]['value'] = $birthdate;
    }
  }
  if (array_key_exists('deat', $indi) && array_key_exists('date', $indi['deat'])) {
    $parts = date_parse($indi['deat']['date']);
    if ($parts['year'] && $parts['month'] && $parts['day']) {
      $deathdate = $parts['year'] . '-' . $parts['month'] . '-' . $parts['day'] . 'T08:00:00';
      $node->person_deathdate['und'][0]['value'] = $deathdate;
    }
  }
  // parse name components
  $given_name = '';
  $surname = '';
  $name_personal = '';
  $title = 'Unknown';
  if (array_key_exists('name', $indi)) {
    if (array_key_exists('name_personal', $indi['name'][0])) {
      $name_personal = $indi['name'][0]['name_personal'];
    }
    if (array_key_exists('namePieceGiven', $indi['name'][0])) {
      $given_name = $indi['name'][0]['namePieceGiven'];
    }
    if (array_key_exists('namePieceSurname', $indi['name'][0])) {
      $surname = $indi['name'][0]['namePieceSurname'];
    }
    if ($given_name) {
      $node->person_given_name[$node->language][0]['value'] = $given_name;
    }
    if ($surname) {
      $node->person_surname[$node->language][0]['value'] = $surname;
    }
    if ($name_personal && !$given_name && !$surname) {
      $nameparts = unkle_gedcom_parse_gedcom_name($name_personal);
      $node->person_given_name[$node->language][0]['value'] = $nameparts['given'];
      $node->person_surname[$node->language][0]['value'] = $nameparts['surname'];
      $surname = $nameparts['surname'];
      $given_name = $nameparts['given'];
    }
    if (!$surname && !$given_name) {
      $surname = 'Unknown';
    }
    $title = unkle_person_title($surname, $given_name, $birthdate, $deathdate);
  }

  $node->title = $title;
  $node->promote = 0;
  node_save($node);
  if ($node->nid) {
    return $node->nid;
  }
  return FALSE;
}

function unkle_gedcom_new_family($fam) {
  if (!($title = $fam['ref'])) {
    $title = 'Unknown';
  }

  $node = new stdClass();
  $node->type = 'unkle_family';
  $node->language = LANGUAGE_NONE;
  node_object_prepare($node);
  $node->title = $title;

  $node->promote = 0;
  node_save($node);
  if ($node->nid) {
    return $node->nid;
  }
  return FALSE;
}

function unkle_gedcom_delete_persons_form($form, &$form_state) {
  $form['warning'] = array(
    '#type' => 'markup',
    '#markup' => '<p>Are you sure you want to delete ALL unkle nodes?</p>',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Delete 'em"),
  );
  return $form;
}

function unkle_gedcom_delete_persons_form_submit($form, &$form_state) {
  $nodes_deleted = unkle_gedcom_delete_all_persons();
  drupal_set_message($nodes_deleted . ' ' . t('Nodes deleted'));
}

function unkle_gedcom_delete_all_persons() {
  $num_deleted = db_delete('node')->condition('type', 'unkle_person')->execute();
  $num_deleted += db_delete('node')->condition('type', 'unkle_family')->execute();
  return $num_deleted;
}

function unkle_gedcom_import_form($form, &$form_state) {
  $form['gedcom_upload_file'] = array(
    '#type' => 'file',
    '#title' => t('Import a GEDCOM file'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );
  return $form;
}

function unkle_gedcom_import_form_validate($form, &$form_state) {
  $validators = array('file_validate_extensions' => array('ged'));
  $file = file_save_upload('gedcom_upload_file', $validators, NULL, FILE_EXISTS_REPLACE);
  if ($file === FALSE) {
    form_set_error('gedcom_upload_file', t("Failed to upload file"));
  }
  elseif ($file !== NULL) {
    $form_state['values']['gedcom_upload_file'] = $file;
  }
  else {
    form_set_error('gedcom_upload_file', t("You must select a file to upload"));
  }
}

function unkle_gedcom_import_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  if ($file = $values['gedcom_upload_file']) {

    cache_set('unkle_gedcom_file', serialize($file), 'cache', time()+21600);

    drupal_goto('gedcom_import_results');
  }
}

function unkle_gedcom_parse_gedcom_name($source) {
  $name = array();
  $surname = substr($source, strpos($source, '/') + 1);
  $surname = substr($surname, 0, strpos($surname, '/'));
  $name['surname'] = $surname;
  $given = substr($source, 0, strpos($source, '/'));
  $name['given'] = $given;
  return $name;
}

