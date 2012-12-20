<?php

/**
 * @file
 * islandora_oai.admin.inc
 *
 */

/**
 * Menu callback. Admin setting page for islandora_oai.
 */
function islandora_oai_settings($form, $form_state) {
  $base_url = $GLOBALS['base_url'];

  $form['#attributes']['enctype'] = 'multipart/form-data';

  //------------ Group 1: Global Configuration
  $form['oai2_configuration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuration'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );
  $form['oai2_configuration']['oai2_repository_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Repository Name'),
    '#required' => TRUE,
    '#size' => '50',
    '#default_value' =>  variable_get('oai2_repository_name', 'Repository')
  );
  $form['oai2_configuration']['oai2_path'] = array(
    '#type' => 'textfield',
    '#title' => t('The path of the Repository'),
    '#field_prefix' => $base_url . '/',
    '#required' => TRUE,
    '#size' => '50',
    '#default_value' => variable_get('oai2_path', 'oai2'),
    '#description' => 'The path where the OAI-PMH service respond. Es. ' . $base_url . '/oai2'
  );
  $form['oai2_configuration']['oai2_repository_identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Repository unique identifier'),
    '#required' => TRUE,
    '#size' => '50',
    '#default_value' => variable_get('oai2_repository_identifier', 'drupal-site.org'),
    '#description' => 'The identifier for this repository. Es. oai:<strong>drupal-site.org</strong>:123.',
  );
  $form['oai2_configuration']['oai2_admin_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Admin Email'),
    '#size' => '50',
    '#default_value' => variable_get('oai2_admin_email', 'admin@example.com')
  );
  $form['oai2_configuration']['oai2_max_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum Response Size'),
    '#size' => '50',
    '#default_value' => variable_get('oai2_max_size', '20'),
    '#description' => 'The maximum number of records to issue per response. If result set is larger than this number,
                      a resumption token will be issued',
  );
  $form['oai2_configuration']['oai2_expire_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Expiration Time'),
    '#size' => '50',
    '#default_value' => variable_get('oai2_expire_time', '86400'),
    '#description' => 'The amount of time a resumption token will remain valid in seconds. Defaults to one day (86400s).',
  );
  $form['oai2_configuration']['oai2_xslt'] = array(
    '#type' => 'textfield',
    '#title' => t('Repository stylesheet XSLT'),
    '#size' => '50',
    '#default_value' => variable_get('oai2_xslt', ''),
    '#description' => 'The XSLT stylesheet for easy browsing the repository without harvester.',
  );
  $form['oai2_configuration']['oai2_date_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr date field'),
    '#size' => '50',
    '#default_value' => variable_get('oai2_date_field', 'fgs.lastModifiedDate'),
    '#description' => 'The date field to use.',
  );
  $form['oai2_configuration']['oai2_collection_field'] = array(
    '#type' => 'textarea',
    '#title' => t('Solr RELS-EXT collection field'),
    '#size' => '50',
    '#default_value' => variable_get('oai2_collection_field', "rels.isMemberOfCollection\nrels.isMemberOf"),
    '#description' => t('Solr fields used to determine if a doc is a member of a collection.  Can also specify a prefix, in the from "field_name ~ prefix".  Ends up with ORing together the list of predicates above into a list like: $field_name:"$prefix$collection_pid"'),
  );
  $form['oai2_configuration']['oai2_xacml_role_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr XACML role field'),
    '#size' => '50',
    '#default_value' => variable_get('oai2_xacml_role_field', 'rels.isViewableByRole'),
    '#description' => 'Field from the RI which is indexed into Solr',
  );
  $form['oai2_configuration']['oai2_xacml_user_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr XACML role field'),
    '#size' => '50',
    '#default_value' => variable_get('oai2_xacml_user_field', 'rels.isViewableByUser'),
    '#description' => 'Field from the RI which is indexed into Solr',
  );
  $form['oai2_configuration']['oai2_content_model_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr hasModel field'),
    '#size' => '50',
    '#default_value' => variable_get('oai2_content_model_field', 'rels.hasModel'),
    '#description' => 'Field which indexes the RELS-EXT hasModel relationship',
  );
  $form['oai2_configuration']['oai2_exclude_content_models'] = array(
    '#type' => 'textarea',
    '#title' => t('Exclude Content Models'),
    '#size' => '50',
    '#default_value' => variable_get('oai2_exclude_content_models', ''),
    '#description' => 'By default all objects are included in the harvest. This field will let you exclude all objects with a certain content model. For example to exclude all objects with the islandora collection content model you would use: "islandora:collectionCModel"',
  );

  //---------- Group: Metadata Format Configuration
  $form['oai2_metadata'] = array(
    '#type' => 'fieldset',
    '#title' => t('Metadata Format'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#prefix' => '<div id="islandora_oai">',
    '#suffix' => '</div>',
  );

  $metadata_format_options = array();
  $result = db_query('SELECT * FROM {oai2_metadata_formats} ORDER BY name');
  foreach ($result as $row) {
    $metadata_format_options[$row->name] = drupal_strtoupper($row->name);
  }

  $metadata_format = variable_get('oai2_metadata_format', 'oai_dc');

  $oai2_options_collapsed = TRUE;

  if (isset($form_state['triggering_element'])) {
    if ($form_state['triggering_element']['#name'] == 'oai2_metadata_format') {
      $metadata_format = $form_state['values']['oai2_metadata_format'];
      $form['oai2_metadata']['#collapsed'] = FALSE;
    }
    // Upload pressed
    else {
      $oai2_options_collapsed = FALSE;

      $validators = array(
        'file_validate_extensions' => array('xslt', 'xsl'),
      );

    }
  }

  $form['oai2_metadata']['oai2_metadata_format'] = array(
    '#type' => 'select',
    '#name' => 'oai2_metadata_format',
    '#title' => t('Metadata Format'),
    '#options' => $metadata_format_options,
    '#default_value' => $metadata_format,
    '#ajax' => array(
      'callback' => 'islandora_oai_select_metadata_prefix_js',
      'wrapper' => 'islandora_oai',
      ),
  );

  $result = db_query("SELECT * FROM {oai2_metadata_formats} WHERE name = :metadata_format",
      array(':metadata_format' => $metadata_format)
  );
  foreach ($result as $row) {
    $metadata_prefix = $row->metadata_prefix;
    $metadata_namespace = $row->metadata_namespace;
    $schema_location = $row->oai2_schema;
  }
  $form['oai2_metadata']['oai2_metadata_prefix'] = array(
    '#type' => 'item',
    '#title' => t('Metadata Prefix'),
    '#markup' =>  $metadata_prefix,
    '#description' => t('Default for selected metadata format.')
  );
  $form['oai2_metadata']['oai2_metadata_namespace'] = array(
    '#type' => 'item',
    '#title' => t('Metadata Namespace'),
    '#markup' => $metadata_namespace,
    '#description' => t('Default for selected metadata format.')
  );
  $form['oai2_metadata']['oai2_schema_location'] = array(
    '#type' => 'item',
    '#title' => t('Schema Location'),
    '#markup' => $schema_location,
    '#description' => t('Default for selected metadata format.')
  );

  $form['oai2_metadata']['oai2_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Transformations'),
    '#collapsible' => TRUE,
    '#collapsed' => $oai2_options_collapsed,
    '#prefix' => '<div id="islandora_oai_options">',
    '#suffix' => '</div>',
  );

  $form['oai2_metadata']['oai2_options']['oai2_metadata_datastream_id_' . $metadata_prefix] = array(
    '#type' => 'textfield',
    '#size' => 30,
    '#title' => 'Metadata Datastream ID',
    '#default_value' => variable_get('oai2_metadata_datastream_id_' . $metadata_prefix, 'MODS'),
    '#description' => t('Note that the datastream ID is case sensitive'),
  );

  $oai_invoke_files = module_invoke_all('islandora_oai_get_xsl_files');

  $transform_options = array('default' => t('No transformation selected'));
  $transform_options = array_merge($transform_options, $oai_invoke_files);

  if ($metadata_prefix == 'oai_dc') {
    $default_transform = variable_get('oai2_transform_file_oai_dc', drupal_get_path('module', 'islandora_oai') . '/xsl/MODS3-22simpleDC.xsl');
  }
  else {
    $default_transform = variable_get('oai2_transform_file_oai_etdms', drupal_get_path('module', 'islandora_oai') . '/xsl/mods_to_etdms.xsl');
  }

  $form['oai2_metadata']['oai2_options']['oai2_transform_file_' . $metadata_prefix] = array(
    '#type' => 'select',
    '#title' => t('File to use for transforming @metadata_prefix', array('@metadata_prefix' => $metadata_prefix)),
    '#options' => $transform_options,
    '#default_value' => $default_transform,
  );

  $form['oai2_metadata']['oai2_options']['upload'] = array(
    '#type' => 'fieldset',
    '#title' => t('Upload a file'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#prefix' => '<div id="islandora_oai_options">',
    '#suffix' => '</div>',
  );

  $form['oai2_metadata']['oai2_options']['upload']['oai2_upload_xsl'] = array(
    '#type' => 'file',
    '#title' => t('Upload a XSL or XSLT to be used for transformations'),
  );

  $form['oai2_metadata']['oai2_options']['upload']['oai2_upload_xsl_button'] = array(
    '#type' => 'submit',
    '#name' => 'oai2_upload_button',
    '#value' => t('Upload file'),
    '#submit' => array('islandora_oai_upload_file'),
    '#title_display' => 'invisible',
    '#size' => 22,
  );

  //---------- Group: Field Mappings
  $form['oai2_field_mapping'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mappings: Field -> DCMI Metadata Term'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t("This is where content type fields are mapped to DCMI Metadata terms. Blank fields will not appear
                         in the OAI-PMH response. If you see no fields here, you must expose some content types above.
                         Field values should be in the form of 'dc:title'<br/><br/>

                        Notes:&nbsp;&nbsp;List of properties in the legacy /elements/1.1/ namespace of Dublin Core:contributor, coverage, creator,
                        date, description, format, identifiers, language, publisher, relation, rights, source, subject, title.<br/><br/>
                        For more information please visit:http://www.dublincore.org/documents/dcmi-terms/<br/><br/>
    "),
  );

  return system_settings_form($form);
}

function islandora_oai_select_metadata_prefix_js($form, &$form_state) {
  return $form['oai2_metadata'];
}

function islandora_oai_upload_file($form, &$form_state) {
  $file = file_save_upload('oai2_upload_xsl', array('file_validate_extensions' => array('xsl xslt')), 'public://islandora_oai_xsls');

  if (!is_null($file)) {
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    drupal_set_message(t("The file @filename was uploaded successfully",
      array(
        '@filename' => $file->filename,
      )
    ));
  }
  else {
    form_set_error('oai2_upload_xsl_submit][oai2_options][upload][oai2_upload_xsl_submit', t('Please select a file to upload'));
  }
}
