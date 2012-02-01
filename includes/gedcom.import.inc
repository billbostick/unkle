<?php

/**
 * @file
 * gedcom import function for the unkle module
 */

function gedcom_parse_file($filename) {

  $ref = '';
  $type = '';
  $indi_count = 0;
  $fam_count = 0;
  $repo_count = 0;
  $source_count = 0;
  $object_count = 0;
  $note_count = 0;
  $subm_count = 0;

  $gedcom = array();
  $indi = array();
  $fam = array();
  $repo = array();
  $sour = array();
  $note = array();
  $obje = array();
  $subm = array();

  ini_set('auto_detect_line_endings', TRUE);
  $gedcom_data = file($filename);

  foreach ($gedcom_data as $nextline) {
    if (trim($nextline[0] == '0')) {
      $level_number = 0;
      if ($ref && $type) {
        switch ($type) {
        case 'INDI':
          $indi_count++;
          $indi['ref'] = $ref;
          gedcom_parse_individual_record($indi, $level_number);
          $gedcom['indi'][] = $indi;
          $indi = array();
          break;
        case 'FAM':
          $fam_count++;
          $fam['ref'] = $ref;
          gedcom_parse_familyRecord($fam, $level_number);
          $gedcom['fam'][] = $fam;
          $fam = array();
          break;
        case 'REPO':
          $repo_count++;
          $repo['ref'] = $ref;
          gedcom_parse_repoRecord($repo, $level_number);
          $gedcom['repo'][] = $repo;
          $repo = array();
          break;
        case 'SOUR':
          $source_count++;
          $sour['ref'] = $ref;
          gedcom_parse_sourceRecord($sour, $level_number);
          $gedcom['sour'][] = $sour;
          $sour = array();
          break;
        case 'NOTE':
          $note_count++;
          $note['ref'] = $ref;
          gedcom_parse_noteRecord($note, $level_number);
          $gedcom['note'][] = $note;
          $note = array();
          break;
        case 'OBJE':
          $object_count++;
          $obje['ref'] = $ref;
          gedcom_parse_objectRecord($obje, $level_number);
          $gedcom['obje'][] = $obje;
          $obje = array();
          break;
        case 'SUBM':
          $subm_count++;
          $subm['ref'] = $ref;
          gedcom_parse_submitterRecord($subm, $level_number);
          $gedcom['subm'][] = $subm;
          $subm = array();
          break;
        }
      }

      if ( (strpos($nextline, 'INDI') === FALSE) &&
           (strpos($nextline, 'FAM') === FALSE) &&
           (strpos($nextline, 'REPO') === FALSE) &&
           (strpos($nextline, 'SOUR') === FALSE) &&
           (strpos($nextline, 'NOTE') === FALSE) &&
           (strpos($nextline, 'OBJE') === FALSE) &&
           (strpos($nextline, 'SUBM') === FALSE)) {
        $type = '';
        $ref = '';
      }
      else {
        $ref = gedcom_extract_reference($nextline);
        if (strpos($nextline, 'INDI')) {
          $type = 'INDI';
        }
        elseif (strpos($nextline, 'FAM')) {
          $type = 'FAM';
        }
        elseif (strpos($nextline, 'REPO')) {
          $type = 'REPO';
        }
        elseif (strpos($nextline, 'SOUR')) {
          $type = 'SOUR';
        }
        elseif (strpos($nextline, 'NOTE')) {
          $type = 'NOTE';
        }
        elseif (strpos($nextline, 'OBJE')) {
          $type = 'OBJE';
        }
        elseif (strpos($nextline, 'SUBM')) {
          $type = 'SUBM';
        }
      }
    }
    else {
      switch ($type) {
      case'FAM':
        $fam['gedcom'][] = $nextline;
        break;
      case'INDI':
        $indi['gedcom'][] = $nextline;
        break;
      case'REPO':
        $repo['gedcom'][] = $nextline;
        break;
      case'SOUR':
        $sour['gedcom'][] = $nextline;
        break;
      case'NOTE':
        $note['gedcom'][] = $nextline;
        break;
      case'OBJE':
        $obje['gedcom'][] = $nextline;
        break;
      case'SUBM':
        $subm['gedcom'][] = $nextline;
        break;
      }
    }
  }

  if ($ref && $type) {
    switch ($type) {
    case 'INDI':
      $indi_count++;
      $indi['ref'] = $ref;
      gedcom_parse_individual_record($indi, 0);
      $gedcom['indi'][] = $indi;
      break;
    case 'FAM':
      $fam_count++;
      $fam['ref'] = $ref;
      gedcom_parse_familyRecord($fam, 0);
      $gedcom['fam'][] = $fam;
      break;
    case 'REPO':
      $repo_count++;
      $repo['ref'] = $ref;
      gedcom_parse_repoRecord($repo, 0);
      $gedcom['repo'][] = $repo;
      break;
    case 'SOUR':
      $source_count++;
      $sour['ref'] = $ref;
      gedcom_parse_sourceRecord($sour, 0);
      $gedcom['sour'][] = $sour;
      break;
    case 'NOTE':
      $note_count++;
      $note['ref'] = $ref;
      gedcom_parse_noteRecord($note, 0);
      $gedcom['note'][] = $note;
      break;
    case 'OBJE':
      $object_count++;
      $obje['ref'] = $ref;
      gedcom_parse_objectRecord($obje, 0);
      $gedcom['obje'][] = $obje;
      break;
    case 'SUBM':
      $subm_count++;
      $subm['ref'] = $ref;
      gedcom_parse_submitterRecord($subm, 0);
      $gedcom['subm'][] = $subm;
      break;
    }
  }
  $gedcom['counts']['indi'] = $indi_count;
  $gedcom['counts']['fam'] = $fam_count;
  $gedcom['counts']['repo'] = $repo_count;
  $gedcom['counts']['sour'] = $source_count;
  $gedcom['counts']['note'] = $note_count;
  $gedcom['counts']['obje'] = $object_count;
  $gedcom['counts']['subm'] = $subm_count;
  return $gedcom;
}

function gedcom_parse_individual_record(&$indi, $level_number = 0) {

  $indi_tags = array(
    'RESN', 'NAME', 'SEX', 
    'BIRT', 'CHR', 'DEAT', 'BURI', 'CREM', 'ADOP', 'BAPM', 'BARM', 'BASM', 
    'BLES', 'CHRA', 'CONF', 'FCOM', 'ORDN', 'NATU', 'EMIG', 'IMMI', 'CENS', 
    'PROB', 'WILL', 'GRAD', 'RETI', 'EVEN',
    'CAST', 'DSCR', 'EDUC', 'INDO', 'NATI', 'NCHI', 'NMR', 'OCCU', 'PROP',
    'RELI', 'RESI', 'RESI', 'SSN', 'TITL', 
    'BAPL', 'CONL', 'ENDL', 'SLGC',
    'FAMC', 'FAMS', 'SUBM', 'ASSO', 'ALIA', 'ANCI', 'DESI', 'SOUR', 'OBJE',
    'NOTE', 'RFN', 'AFN', 'REFN', 'RIN', 'CHAN', 
   );

  $indi_event_tags = array(
    'BIRT', 'CHR', 'DEAT', 'BURI', 'CREM', 'ADOP', 'BAPM', 'BARM', 'BASM', 
    'BLES', 'CHRA', 'CONF', 'FCOM', 'ORDN', 'NATU', 'EMIG', 'IMMI', 'CENS', 
    'PROB', 'WILL', 'GRAD', 'RETI', 'EVEN',
  );

  $name_personal = '';
  $type = '';
  $resn_count = 0;
  $name_count = 0;

  $name = array();
  $user_ref = array();
  $change_date = array();
  $childtofam = array();
  $spousetofam = array();
  $note = array();
  $association = array();
  $attribute = array();
  $event = array();

  $sublevel = '' . ($level_number + 1);
   
  foreach ($indi['gedcom'] as $nextline) {
    if (trim($nextline[0] == $sublevel)) {
    // we're reading a top-level tag
      if ($type) {
      // it $type has a value, then we need to clean up for the last top-level tag
        switch ($type) {
        case 'NAME':
          $name_count++;
          $name['name_personal'] = $name_personal;
          if (array_key_exists('gedcom', $name)) {
            gedcom_parse_personalNameStructure($name, $sublevel);
          }
          $indi['name'][] = $name;
          $name = array();
          break;
        case 'NOTE':
          $note['text'] = $note_text;
          if (array_key_exists('gedcom', $note)) {
            gedcom_parse_concatStructure($note, $sublevel);
          }
          $indi['note'][] = $note;
          $note = array();
          break;
        case 'ASSO':
          $association['ref'] = $association_link;
          if (array_key_exists('gedcom', $association)) {
            gedcom_parse_associationStructure($association, $sublevel);
          }
          $indi['asso'][] = $association;
          $association = array();
          break;
        case 'IEVENT':
          $event['type'] = $event_type;
          if (array_key_exists('gedcom', $event)) {
            gedcom_parse_individualEventStructure($event, $sublevel);
          }
          $indi[strtolower($event_type)] = $event;
          $event = array();
          break;
        case 'REFN':
          $user_ref['refn'] = $user_ref_num;
          if (array_key_exists('gedcom', $user_ref)) {
            gedcom_parse_user_referenceStructure($user_ref, $sublevel);
          }
          $indi['refn'][] = $user_ref;
          $user_ref = array();
          break;
        case 'DATE':
          $change_date['date'] = $change_date;
          if (array_key_exists('gedcom', $change_date)) {
            gedcom_parse_change_dateStructure($change_date, $sublevel);
          }
          $indi['changedate'] = $change_date;
          $change_date = array();
          break;
        case 'FAMC':
          $childtofam['ref'] = $childtofam_link;
          if (array_key_exists('gedcom', $childtofam)) {
            gedcom_parse_childtofamStructure($childtofam, $sublevel);
          }
          $indi['famc'][] = $childtofam;
          $childtofam = array();
          break;
        case 'FAMS':
          $spousetofam['ref'] = $spousetofam_link;
          if (array_key_exists('gedcom', $spousetofam)) {
            gedcom_parse_spousetofamStructure($spousetofam, $sublevel);
          }
          $indi['fams'][] = $spousetofam;
          $spousetofam = array();
          break;
        }
      }

      if (!gedcom_contains_tag($nextline, $indi_tags)) {
      // not a known top-level tag
        $type = '';
        $name_personal = '';
        $user_ref_num = '';
        $chage_date = '';
        $childtofam_link = '';
        $spousetofam_link = '';
        $note_text = '';
        $association_ref = '';
        $event_type = '';
      }
      else {
      // process top-level tag

        if ($event_type = gedcom_contains_tag($nextline, $indi_event_tags)) {
          $type = 'IEVENT';
        }
        elseif (strpos($nextline, 'RESN')) {
          $indi['resn'] = substr(trim($nextline), 7);
          $type = '';
        }
        elseif (strpos($nextline, 'SUBM')) {
          $indi['subm'][] = gedcom_extract_reference($nextline);
          $type = '';
        }
        elseif (strpos($nextline, 'ALIA')) {
          $indi['alia'][] = gedcom_extract_reference($nextline);
          $type = '';
        }
        elseif (strpos($nextline, 'ANCI')) {
          $indi['anci'][] = gedcom_extract_reference($nextline);
          $type = '';
        }
        elseif (strpos($nextline, 'DESI')) {
          $indi['desi'][] = gedcom_extract_reference($nextline);
          $type = '';
        }

        elseif (strpos($nextline, 'RFN')) {
          $indi['rfn'] = substr(trim($nextline), 6);
          $type = '';
        }
        elseif (strpos($nextline, 'AFN')) {
          $indi['afn'] = substr(trim($nextline), 6);
          $type = '';
        }
        elseif (strpos($nextline, 'SEX')) {
          $indi['sex'] = substr(trim($nextline), 6);
          $type = '';
        }
        elseif (strpos($nextline, 'RIN')) {
          $indi['rin'] = substr(trim($nextline), 6);
          $type = '';
        }
        elseif (strpos($nextline, 'FAMC')) {
          $childtofam_link = gedcom_extract_reference($nextline);
          $type = 'FAMC';
        }
        elseif (strpos($nextline, 'FAMS')) {
          $spousetofam_link = gedcom_extract_reference($nextline);
          $type = 'FAMS';
        }
        elseif (strpos($nextline, 'ASSO')) {
          $association_link = gedcom_extract_reference($nextline);
          $type = 'ASSO';
        }
        elseif (strpos($nextline, 'REFN')) {
          $user_ref_num = substr(trim($nextline), 7);
          $type = 'REFN';
        }
        elseif (strpos($nextline, 'DATE')) {
          $change_date = substr(trim($nextline), 7);
          $type = 'DATE';
        }
        elseif (strpos($nextline, 'NOTE')) {
          if (trim($nextline[7] == '@')) {
            $indi['note'][] = gedcom_extract_reference($nextline);
            $type = '';
          }
          else {
            $note_text = substr(trim($nextline), 7);
            $type = 'NOTE';
          }
        }
        elseif (strpos($nextline, 'NAME')) {
          $name_personal = substr(trim($nextline), 7);
          $type = 'NAME';
        }
      }
    }
    else {
    // we're at an inner level, so populate the 'gedcom' field for the parent tag
      switch ($type) {
      case 'NAME':
        $name['gedcom'][] = $nextline;
        break;
      case 'ASSO':
        $association['gedcom'][] = $nextline;
        break;
      case 'REFN':
        $user_ref['gedcom'][] = $nextline;
        break;
      case 'DATE':
        $change_date['gedcom'][] = $nextline;
        break;
      case 'FAMC':
        $childtofam['gedcom'][] = $nextline;
        break;
      case 'FAMS':
        $spousetofam['gedcom'][] = $nextline;
        break;
      case 'NOTE':
        $note['gedcom'][] = $nextline;
        break;
      case 'IEVENT':
        $event['gedcom'][] = $nextline;
        break;
      }
    }  
  }

  // done with the records, clean up any active top-level tag
  if ($type) {
    switch ($type) {
    case 'NAME':
      $name_count++;
      $name['name_personal'] = $name_personal;
      if (array_key_exists('gedcom', $name)) {
        gedcom_parse_personalNameStructure($name, 1);
      }
      $indi['name'][] = $name;
      break;
    case 'ASSO':
      $association['ref'] = $association_link;
      if (array_key_exists('gedcom', $association)) {
        gedcom_parse_associationStructure($association, 1);
      }
      $indi['asso'][] = $association;
      break;
    case 'REFN':
      $user_ref['refn'] = $user_ref_num;
      if (array_key_exists('gedcom', $user_ref)) {
        gedcom_parse_user_referenceStructure($user_ref, 1);
      }
      $indi['refn'][] = $user_ref;
      break;
    case 'DATE':
      $change_date['date'] = $change_date;
      if (array_key_exists('gedcom', $change_date)) {
        gedcom_parse_change_dateStructure($change_date, 1);
      }
      $indi['changedate'] = $change_date;
      break;
    case 'NOTE':
      $note['text'] = $note_text;
      if (array_key_exists('gedcom', $note)) {
        gedcom_parse_concatStructure($note, 1);
      }
      $indi['note'][] = $note;
      break;
    case 'FAMC':
      $childtofam['ref'] = $childtofam_link;
      if (array_key_exists('gedcom', $childtofam)) {
        gedcom_parse_childtofamStructure($childtofam, 1);
      }
      $indi['famc'][] = $childtofam;
      break;
    case 'FAMs':
      $spousetofam['ref'] = $spousetofam_link;
      if (array_key_exists('gedcom', $spousetofam)) {
        gedcom_parse_spousetofamStructure($spousetofam, 1);
      }
      $indi['fams'][] = $spousetofam;
      break;
    }
  }
}

function gedcom_parse_familyRecord(&$fam, $level_number = 0) {
  $type = '';
  $user_ref = array();
  $change_date = array();
  $note = array();

  $sublevel = '' . ($level_number + 1);
   
  foreach ($fam['gedcom'] as $nextline) {
    if (trim($nextline[0] == $sublevel)) {

      if ($type) {
        switch ($type) {
        case 'NOTE':
          $note['text'] = $note_text;
          if (array_key_exists('gedcom', $note)) {
            gedcom_parse_concatStructure($note, $sublevel);
          }
          $fam['note'][] = $note;
          $note = array();
          break;
        case 'REFN':
          $user_ref['refn'] = $user_ref_num;
          if (array_key_exists('gedcom', $user_ref)) {
            gedcom_parse_user_referenceStructure($user_ref, $sublevel);
          }
          $fam['refn'][] = $user_ref;
          $user_ref = array();
          break;
        case 'DATE':
          $change_date['date'] = $change_date;
          if (array_key_exists('gedcom', $change_date)) {
            gedcom_parse_change_dateStructure($change_date, $sublevel);
          }
          $fam['changedate'] = $change_date;
          $change_date = array();
          break;
        }
      }

      if ( (strpos($nextline, 'HUSB') === FALSE) &&
           (strpos($nextline, 'WIFE') === FALSE) &&
           (strpos($nextline, 'NOTE') === FALSE) &&
           (strpos($nextline, 'REFN') === FALSE) &&
           (strpos($nextline, 'RIN') === FALSE) &&
           (strpos($nextline, 'NCHI') === FALSE) &&
           (strpos($nextline, 'DATE') === FALSE) &&
           (strpos($nextline, 'SUBM') === FALSE) &&
           (strpos($nextline, 'CHIL') === FALSE)) {
        $type = '';
        $note_text = '';
        $user_ref_nom = '';
        $change_date = '';
      }
      else {
        if (strpos($nextline, 'HUSB')) {
          $fam['husb'] = gedcom_extract_reference($nextline);
          $type = '';
        }

        elseif (strpos($nextline, 'WIFE')) {
          $fam['wife'] = gedcom_extract_reference($nextline);
          $type = '';
        }
        elseif (strpos($nextline, 'CHIL')) {
          $fam['chil'][] = gedcom_extract_reference($nextline);
          $type = '';
        }
        elseif (strpos($nextline, 'RESN')) {
          $fam['resn'] = substr(trim($nextline), 7);
          $type = '';
        }

        elseif (strpos($nextline, 'SUBM')) {
          $fam['subm'][] = gedcom_extract_reference($nextline);
          $type = '';
        }
        elseif (strpos($nextline, 'RIN')) {
          $fam['rin'] = substr(trim($nextline), 6);
          $type = '';
        }
        elseif (strpos($nextline, 'DATE')) {
          $change_date = substr(trim($nextline), 7);
          $type = 'DATE';
        }
        elseif (strpos($nextline, 'NOTE')) {
          if (trim($nextline[7] == '@')) {
            $fam['note'][] = gedcom_extract_reference($nextline);
            $type = '';
          }
          else {
            $note_text = substr(trim($nextline), 7);
            $type = 'NOTE';
          }
        }
        elseif (strpos($nextline, 'REFN')) {
          $user_ref_num = substr(trim($nextline), 7);
          $type = 'REFN';
        }
        elseif (strpos($nextline, 'NCHI')) {
          $fam['nchi'] = substr(trim($nextline), 7);
          $type = '';
        }
      }
    }
    else {
      switch ($type) {
      case 'DATE':
        $change_date['gedcom'][] = $nextline;
        break;
      case 'NOTE':
        $note['gedcom'][] = $nextline;
        break;
      case 'REFN':
        $user_ref['gedcom'][] = $nextline;
        break;
      }
    }  
  }
  if ($type) {
    switch ($type) {
    case 'DATE':
      $change_date['date'] = $change_date;
      if (array_key_exists('gedcom', $change_date)) {
        gedcom_parse_change_dateStructure($change_date, 1);
      }
      $fam['changedate'] = $change_date;
      break;
    case 'NOTE':
      $note['text'] = $note_text;
      if (array_key_exists('gedcom', $note)) {
        gedcom_parse_concatStructure($note, 1);
      }
      $fam['note'][] = $note;
      break;
    case 'REFN':
      $user_ref['refn'] = $user_ref_num;
      if (array_key_exists('gedcom', $user_ref)) {
        gedcom_parse_user_referenceStructure($user_ref, 1);
      }
      $fam['refn'][] = $user_ref;
      break;
    }
  }
}

function gedcom_parse_sourceRecord(&$sour, $level_number = 0) {
  $type = '';
  $user_ref = array();
  $auth = array();
  $note = array();
  $title = array();
  $pubfacts = array();
  $change_date = array();
  $sourcetext = array();

  $sublevel = '' . ($level_number + 1);
   
  foreach ($sour['gedcom'] as $nextline) {
    if (trim($nextline[0] == $sublevel)) {

      if ($type) {
        switch ($type) {
        case 'NOTE':
          $note['text'] = $note_text;
          if (array_key_exists('gedcom', $note)) {
            gedcom_parse_concatStructure($note, $sublevel);
          }
          $sour['note'][] = $note;
          $note = array();
          break;
        case 'TITL':
          $title['text'] = $title_text;
          if (array_key_exists('gedcom', $title)) {
            gedcom_parse_concatStructure($title, $sublevel);
          }
          $sour['title'] = $title;
          $title = array();
          break;
        case 'AUTH':
          $auth['orig'] = $auth_text;
          if (array_key_exists('gedcom', $auth)) {
            gedcom_parse_concatStructure($auth, $sublevel);
          }
          $sour['auth'] = $auth;
          $auth = array();
          break;
        case 'PUBL':
          $pubfacts['text'] = $pubfacts_text;
          if (array_key_exists('gedcom', $pubfacts)) {
            gedcom_parse_concatStructure($pubfacts, $sublevel);
          }
          $sour['publ'] = $pubfacts;
          $pubfacts = array();
          break;
        case 'TEXT':
          $sourcetext['text'] = $source_text;
          if (array_key_exists('gedcom', $sourcetext)) {
            gedcom_parse_concatStructure($sourcetext, $sublevel);
          }
          $sour['text'] = $sourcetext;
          $sourcetext = array();
          break;
        case 'REFN':
          $user_ref['refn'] = $user_ref_num;
          if (array_key_exists('gedcom', $user_ref)) {
            gedcom_parse_user_referenceStructure($user_ref, $sublevel);
          }
          $sour['refn'][] = $user_ref;
          $user_ref = array();
          break;
        case 'DATE':
          $change_date['date'] = $change_date;
          if (array_key_exists('gedcom', $change_date)) {
            gedcom_parse_change_dateStructure($change_date, $sublevel);
          }
          $sour['changedate'] = $change_date;
          $change_date = array();
          break;
        }
      }

      if ( (strpos($nextline, 'REFN') === FALSE) &&
           (strpos($nextline, 'NOTE') === FALSE) &&
           (strpos($nextline, 'TITL') === FALSE) &&
           (strpos($nextline, 'AUTH') === FALSE) &&
           (strpos($nextline, 'PUBL') === FALSE) &&
           (strpos($nextline, 'TEXT') === FALSE) &&
           (strpos($nextline, 'RIN') === FALSE) &&
           (strpos($nextline, 'ABBR') === FALSE) &&
           (strpos($nextline, 'DATE') === FALSE)) {
        $type = '';
        $note_text = '';
        $user_ref_num = '';
        $change_date = '';
        $auth_text = '';
        $title_text = '';
        $pubfacts_text = '';
        $source_text = '';
      }
      else {
        if (strpos($nextline, 'AUTH')) {
          $auth_text = substr(trim($nextline), 7);
          $type = 'AUTH';
        }
        elseif (strpos($nextline, 'TITL')) {
          $title_text = substr(trim($nextline), 7);
          $type = 'TITL';
        }
        elseif (strpos($nextline, 'PUBL')) {
          $pubfacts_text = substr(trim($nextline), 7);
          $type = 'PUBL';
        }
        elseif (strpos($nextline, 'TEXT')) {
          $source_text = substr(trim($nextline), 7);
          $type = 'TEXT';
        }
        elseif (strpos($nextline, 'NOTE')) {
          if (trim($nextline[7] == '@')) {
            $sour['note'][] = gedcom_extract_reference($nextline);
            $type = '';
          }
          else {
            $note_text = substr(trim($nextline), 7);
            $type = 'NOTE';
          }
        }
        elseif (strpos($nextline, 'ABBR')) {
          $sour['abbr'] = substr(trim($nextline), 6);
          $type = '';
        }
        elseif (strpos($nextline, 'RIN')) {
          $sour['rin'] = substr(trim($nextline), 6);
          $type = '';
        }
        elseif (strpos($nextline, 'DATE')) {
          $change_date = substr(trim($nextline), 7);
          $type = 'DATE';
        }
        elseif (strpos($nextline, 'REFN')) {
          $user_ref_num = substr(trim($nextline), 7);
          $type = 'REFN';
        }
      }
    }
    else {
      switch ($type) {
      case 'NOTE':
        $note['gedcom'][] = $nextline;
        break;
      case 'TITL':
        $title['gedcom'][] = $nextline;
        break;
      case 'AUTH':
        $auth['gedcom'][] = $nextline;
        break;
      case 'PUBL':
        $pubfacts['gedcom'][] = $nextline;
        break;
      case 'TEXT':
        $sourcetext['gedcom'][] = $nextline;
        break;
      case 'DATE':
        $change_date['gedcom'][] = $nextline;
        break;
      case 'REFN':
        $user_ref['gedcom'][] = $nextline;
        break;
      }
    }  
  }
  if ($type) {
    switch ($type) {
    case 'NOTE':
      $note['text'] = $note_text;
      if (array_key_exists('gedcom', $note)) {
        gedcom_parse_concatStructure($note, 1);
      }
      $sour['note'][] = $note;
      break;
    case 'TITL':
      $title['text'] = $auth_text;
      if (array_key_exists('gedcom', $title)) {
        gedcom_parse_concatStructure($title, 1);
      }
      $sour['titl'] = $title;
      break;
    case 'AUTH':
      $auth['orig'] = $auth_text;
      if (array_key_exists('gedcom', $auth)) {
        gedcom_parse_concatStructure($auth, 1);
      }
      $sour['auth'] = $auth;
      break;
    case 'PUBL':
      $auth['publ'] = $pubfacts_text;
      if (array_key_exists('gedcom', $pubfacts)) {
        gedcom_parse_concatStructure($pubfacts, 1);
      }
      $sour['publ'] = $pubfacts;
      break;
    case 'TEXT':
      $sourcetext['text'] = $source_text;
      if (array_key_exists('gedcom', $sourcetext)) {
        gedcom_parse_concatStructure($sourcetext, 1);
      }
      $sour['text'] = $sourcetext;
      break;
    case 'DATE':
      $change_date['date'] = $change_date;
      if (array_key_exists('gedcom', $change_date)) {
        gedcom_parse_change_dateStructure($change_date, 1);
      }
      $sour['changedate'] = $change_date;
      break;
    case 'REFN':
      $user_ref['refn'] = $user_ref_num;
      if (array_key_exists('gedcom', $user_ref)) {
        gedcom_parse_user_referenceStructure($user_ref, 1);
      }
      $sour['refn'][] = $user_ref;
      break;
    }
  }
}

function gedcom_parse_objectRecord(&$obje, $level_number = 0) {
  $type = '';
  $user_ref = array();
  $change_date = array();

  $sublevel = '' . ($level_number + 1);
  foreach ($obje['gedcom'] as $nextline) {
    if (trim($nextline[0] == $sublevel)) {

      if ($type) {
        switch ($type) {
        case 'REFN':
          $user_ref['refn'] = $user_ref_num;
          if (array_key_exists('gedcom', $user_ref)) {
            gedcom_parse_user_referenceStructure($user_ref, $sublevel);
          }
          $obje['refn'][] = $user_ref;
          $user_ref = array();
          break;
        case 'DATE':
          $change_date['date'] = $change_date;
          if (array_key_exists('gedcom', $change_date)) {
            gedcom_parse_change_dateStructure($change_date, $sublevel);
          }
          $obje['changedate'] = $change_date;
          $change_date = array();
          break;
        }
      }
      if ( (strpos($nextline, 'DATE') === FALSE) &&
           (strpos($nextline, 'FORM') === FALSE) &&
           (strpos($nextline, 'TITL') === FALSE) &&
           (strpos($nextline, 'OBJE') === FALSE) &&
           (strpos($nextline, 'REFN') === FALSE) &&
           (strpos($nextline, 'RIN') === FALSE)) {
        $type = '';
        $user_ref_num = '';
        $change_date = '';
      }
      else {
        if (strpos($nextline, 'DATE')) {
          $change_date = substr(trim($nextline), 7);
          $type = 'DATE';
        }
        elseif (strpos($nextline, 'FORM')) {
          $obje['rin'] = substr(trim($nextline), 7);
          $type = '';
        }
        elseif (strpos($nextline, 'TITL')) {
          $obje['rin'] = substr(trim($nextline), 7);
          $type = '';
        }
        elseif (strpos($nextline, 'OBJE')) {
          $obje['obje'] = gedcom_extract_reference($nextline);
          $type = '';
        }
        elseif (strpos($nextline, 'RIN')) {
          $obje['rin'] = substr(trim($nextline), 6);
          $type = '';
        }
        elseif (strpos($nextline, 'REFN')) {
          $user_ref_num = substr(trim($nextline), 7);
          $type = 'REFN';
        }
      }
    }
    else {
      switch ($type) {
      case 'DATE':
        $change_date['gedcom'][] = $nextline;
        break;
      case 'REFN':
        $user_ref['gedcom'][] = $nextline;
        break;
      }
    }  
  }
  if ($type) {
    switch ($type) {
    case 'DATE':
      $change_date['date'] = $change_date;
      if (array_key_exists('gedcom', $change_date)) {
        gedcom_parse_change_dateStructure($change_date, 1);
      }
      $obje['changedate'] = $change_date;
      break;
    case 'REFN':
      $user_ref['refn'] = $user_ref_num;
      if (array_key_exists('gedcom', $user_ref)) {
        gedcom_parse_user_referenceStructure($user_ref, 1);
      }
      $obje['refn'][] = $user_ref;
      break;
    }
  }
}

function gedcom_parse_noteRecord(&$note, $level_number = 0) {
  $type = '';
  $user_ref = array();
  $change_date = array();

  $sublevel = '' . ($level_number + 1);
  foreach ($note['gedcom'] as $nextline) {
    if (trim($nextline[0] == $sublevel)) {

      if ($type) {
        switch ($type) {
        case 'REFN':
          $user_ref['refn'] = $user_ref_num;
          if (array_key_exists('gedcom', $user_ref)) {
            gedcom_parse_user_referenceStructure($user_ref, $sublevel);
          }
          $note['refn'][] = $user_ref;
          $user_ref = array();
          break;
        case 'DATE':
          $change_date['date'] = $change_date;
          if (array_key_exists('gedcom', $change_date)) {
            gedcom_parse_change_dateStructure($change_date, $sublevel);
          }
          $note['changedate'] = $change_date;
          $change_date = array();
          break;
        }
      }
      if ( (strpos($nextline, 'DATE') === FALSE) &&
           (strpos($nextline, 'REFN') === FALSE) &&
           (strpos($nextline, 'RIN') === FALSE)) {
        $type = '';
        $user_ref_num = '';
        $change_date = '';
      }
      else {
        if (strpos($nextline, 'DATE')) {
          $change_date = substr(trim($nextline), 7);
          $type = 'DATE';
        }
        elseif (strpos($nextline, 'RIN')) {
          $note['rin'] = substr(trim($nextline), 6);
          $type = '';
        }
        elseif (strpos($nextline, 'REFN')) {
          $user_ref_num = substr(trim($nextline), 7);
          $type = 'REFN';
        }
      }
    }
    else {
      switch ($type) {
      case 'DATE':
        $change_date['gedcom'][] = $nextline;
        break;
      case 'REFN':
        $user_ref['gedcom'][] = $nextline;
        break;
      }
    }  
  }
  if ($type) {
    switch ($type) {
    case 'DATE':
      $change_date['date'] = $change_date;
      if (array_key_exists('gedcom', $change_date)) {
        gedcom_parse_change_dateStructure($change_date, 1);
      }
      $note['changedate'] = $change_date;
      break;
    case 'REFN':
      $user_ref['refn'] = $user_ref_num;
      if (array_key_exists('gedcom', $user_ref)) {
        gedcom_parse_user_referenceStructure($user_ref, 1);
      }
      $note['refn'][] = $user_ref;
      break;
    }
  }
}

function gedcom_parse_repoRecord(&$repo, $level_number = 0) {
  $type = '';
  $user_ref = array();
  $change_date = array();

  $sublevel = '' . ($level_number + 1);

  foreach ($repo['gedcom'] as $nextline) {
    if (trim($nextline[0] == $sublevel)) {

      if ($type) {
        switch ($type) {
        case 'REFN':
          $user_ref['refn'] = $user_ref_num;
          if (array_key_exists('gedcom', $user_ref)) {
            gedcom_parse_user_referenceStructure($user_ref, $sublevel);
          }
          $repo['refn'][] = $user_ref;
          $user_ref = array();
          break;
        case 'DATE':
          $change_date['date'] = $change_date;
          if (array_key_exists('gedcom', $change_date)) {
            gedcom_parse_change_dateStructure($change_date, $sublevel);
          }
          $repo['changedate'] = $change_date;
          $change_date = array();
          break;
        }
      }
      if ( (strpos($nextline, 'NAME') === FALSE) &&
           (strpos($nextline, 'DATE') === FALSE) &&
           (strpos($nextline, 'REFN') === FALSE) &&
           (strpos($nextline, 'RIN') === FALSE)) {
        $type = '';
        $user_ref_num = '';
        $change_date = '';
      }
      else {
        if (strpos($nextline, 'NAME')) {
          $repo['name'] = substr(trim($nextline), 6);
          $type = '';
        }
        elseif (strpos($nextline, 'DATE')) {
          $change_date = substr(trim($nextline), 7);
          $type = 'DATE';
        }
        elseif (strpos($nextline, 'RIN')) {
          $repo['rin'] = substr(trim($nextline), 6);
          $type = '';
        }
        elseif (strpos($nextline, 'REFN')) {
          $user_ref_num = substr(trim($nextline), 7);
          $type = 'REFN';
        }
      }
    }
    else {
      switch ($type) {
      case 'DATE':
        $change_date['gedcom'][] = $nextline;
        break;
      case 'REFN':
        $user_ref['gedcom'][] = $nextline;
        break;
      }
    }  
  }
  if ($type) {
    switch ($type) {
    case 'DATE':
      $change_date['date'] = $change_date;
      if (array_key_exists('gedcom', $change_date)) {
        gedcom_parse_change_dateStructure($change_date, 1);
      }
      $repo['changedate'] = $change_date;
      break;
    case 'REFN':
      $user_ref['refn'] = $user_ref_num;
      if (array_key_exists('gedcom', $user_ref)) {
        gedcom_parse_user_referenceStructure($user_ref, 1);
      }
      $repo['refn'][] = $user_ref;
      break;
    }
  }
}

function gedcom_extract_reference($source) {
  $ref = substr($source, strpos($source, '@') + 1);
  $ref = substr($ref, 0, strpos($ref, '@'));
  return $ref;
}

function gedcom_parsePersonalNamePart($source) {
  return (substr(trim($source), 7));
}

function gedcom_parse_associationStructure(&$ref, $level_number) {
  foreach ($ref['gedcom'] as $nextline) {
    if (strpos($nextline, 'RELA')) {
      $ref['rela'] = substr(trim($nextline), 7);
    }
  }
  return;
}

function gedcom_parse_individualEventStructure(&$event, $level_number) {
  foreach ($event['gedcom'] as $nextline) {
    if (strpos($nextline, 'DATE')) {
      $event['date'] = substr(trim($nextline), 7);
    }
  }
  return;
}

function gedcom_parse_user_referenceStructure(&$ref, $level_number) {
  foreach ($ref['gedcom'] as $nextline) {
    if (strpos($nextline, 'TYPE')) {
      $ref['type'] = substr(trim($nextline), 7);
    }
  }
  return;
}

function gedcom_parse_concatStructure(&$struct, $level_number) {
  foreach ($struct['gedcom'] as $nextline) {
    if (strpos($nextline, 'CONT')) {
      $struct['cont'][] = substr(trim($nextline), 7);
    }
    if (strpos($nextline, 'CONC')) {
      $struct['conc'][] = substr(trim($nextline), 7);
    }
  }
  return;
}

function gedcom_parse_change_dateStructure(&$changedate, $level_number) {
  foreach ($changedate['gedcom'] as $nextline) {
    if (strpos($nextline, 'TIME')) {
      $changedate['time'] = substr(trim($nextline), 7);
    }
  }
  return;
}

function gedcom_parse_childtofamStructure(&$childtofam, $level_number) {
  foreach ($childtofam['gedcom'] as $nextline) {
    if (strpos($nextline, 'PEDI')) {
      $childtofam['pedi'] = substr(trim($nextline), 7);
    }
  }
  return;
}

function gedcom_parse_spousetofamStructure(&$spousetofam, $level_number) {
  return;
}

function gedcom_parse_personalNameStructure(&$name, $level_number) {
  foreach ($name['gedcom'] as $nextline) {
    if (strpos($nextline, 'NPFX')) {
      $name['namePiecePrefix'] = gedcom_ParsePersonalNamePart($nextline);
    }
    elseif (strpos($nextline, 'GIVN')) {
      $name['namePieceGiven'] = gedcom_ParsePersonalNamePart($nextline);
    }
    elseif (strpos($nextline, 'NICK')) {
      $name['namePieceNickname'] = gedcom_ParsePersonalNamePart($nextline);
    }
    elseif (strpos($nextline, 'SPFX')) {
      $name['namePieceSurnamePrefix'] = gedcom_ParsePersonalNamePart($nextline);
    }
    elseif (strpos($nextline, 'SURN')) {
      $name['namePieceSurname'] = gedcom_ParsePersonalNamePart($nextline);
    }
    elseif (strpos($nextline, 'NSFX')) {
      $name['namePieceSuffix'] = gedcom_ParsePersonalNamePart($nextline);
    }
  }
  return;
}

function gedcom_contains_tag($source, $tags) {
  if (is_array($tags)) {
    foreach ($tags as $tag) {
      if (strpos($source, $tag) !== FALSE) {
        return $tag;
      }
    }
  }
  return FALSE;
}

function gedcom_parse_submitterRecord(&$subm, $level_number = 0) {
}

