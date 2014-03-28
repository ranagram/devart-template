<?php

class Controller_Words extends Controller_Rest
{

	public function post_regist()
	{
 		$val = Model_Word::validate('regist');
		
		$result = array('result' => 1);	
		if ($val->run())
		{
			$word = Model_Word::forge(array(
				'content' => $val->validated('content'),
				'name' => $val->validated('name'),
			));
						
			if ($word and $word->save())
			{
			}
			else
			{
				$result = array('result' => 0);
			}
		}else{
			$result = array('result' => 0);
		}

      	return $this->response($result);
	}

	public function get_list()
	{
		$words = Model_Word::find('all', array('order_by' => array('id' => 'desc'), 'limit' => 25));

		$data = array();
		foreach($words as $word){
			array_push($data, array('name' => $word->name, 'word' => $word->content));
		}

       	return $this->response($data);
	}

}
