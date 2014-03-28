<?php

class Model_Word extends \Orm\Model
{
	protected static $_properties = array(
		'id',
		'content',
		'name',
		'created_at',
		'updated_at',
	);

	protected static $_observers = array(
		'Orm\Observer_CreatedAt' => array(
			'events' => array('before_insert'),
			'mysql_timestamp' => false,
		),
		'Orm\Observer_UpdatedAt' => array(
			'events' => array('before_update'),
			'mysql_timestamp' => false,
		),
	);
	protected static $_table_name = 'words';

	public static function validate($factory)
	{
		$val = Validation::forge($factory);

		$val->add_field('name', 'name', 'trim|required');
		$val->add_field('content', 'word', 'trim|required');

		return $val;
	}
}
