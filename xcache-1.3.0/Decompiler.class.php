<?php

define('INDENT', "\t");
ini_set('error_reporting', E_ALL);

function color($str, $color = 33)
{
	return "\x1B[{$color}m$str\x1B[0m";
}

function str($src, $indent = '') // {{{
{
	if (is_array($indent)) {
		$indent = $indent['indent'];
	}

	/*
	$e = xcache_get_special_value($src);
	if (isset($e)) {
		if (is_array($e)) {
			$src = $e;
		}
		else {
			return $e;
		}
	}
	*/

	if (is_array($src)) {
		die_error('array str');
		$src = new Decompiler_Array($src, false, $indent);
		return $src->__toString();
	}

	if (is_object($src)) {
		if (!method_exists($src, '__toString')) {
			var_dump($src);
			die_error('no __toString');
		}
		return $src->__toString($indent);
	}

	return $src;
}
// }}}
function value($value) // {{{
{
	$spec = xcache_get_special_value($value);
	if (isset($spec)) {
		$value = $spec;
		if (!is_array($value)) {
			// constant
			return $value;
		}
	}

	if (is_array($value)) {
		$value = new Decompiler_Array($value, true);
	}
	else {
		$value = new Decompiler_Value($value, true);
	}
	return $value;
}
// }}}
class Decompiler_Object // {{{
{
}
// }}}
class Decompiler_Value extends Decompiler_Object // {{{
{
	var $value;

	function Decompiler_Value($value = null)
	{
		$this->value = $value;
	}

	function __toString()
	{
		return var_export($this->value, true);
	}
}
// }}}
class Decompiler_Code extends Decompiler_Object // {{{
{
	var $src;

	function Decompiler_Code($src)
	{
		$this->src = $src;
	}

	function __toString()
	{
		return $this->src;
	}
}
// }}}
class Decompiler_Binop extends Decompiler_Code // {{{
{
	var $opc;
	var $op1;
	var $op2;
	var $parent;

	function Decompiler_Binop($parent, $op1, $opc, $op2)
	{
		$this->parent = &$parent;
		$this->opc = $opc;
		$this->op1 = $op1;
		$this->op2 = $op2;
	}

	function __toString()
	{
		$op1 = str($this->op1);
		if (is_a($this->op1, 'Decompiler_Binop') && $this->op1->opc != $this->opc) {
			$op1 = "($op1)";
		}
		$opstr = $this->parent->binops[$this->opc];
		if ($op1 == '0' && $this->opc == XC_SUB) {
			return $opstr . str($this->op2);
		}
		return $op1 . ' ' . $opstr . ' ' . str($this->op2);
	}
}
// }}}
class Decompiler_Fetch extends Decompiler_Code // {{{
{
	var $src;
	var $fetchType;

	function Decompiler_Fetch($src, $type, $globalsrc)
	{
		$this->src = $src;
		$this->fetchType = $type;
		$this->globalsrc = $globalsrc;
	}

	function __toString()
	{
		switch ($this->fetchType) {
		case ZEND_FETCH_LOCAL:
			return '$' . substr($this->src, 1, -1);
		case ZEND_FETCH_STATIC:
			die('static fetch cant to string');
		case ZEND_FETCH_GLOBAL:
		case ZEND_FETCH_GLOBAL_LOCK:
			return $this->globalsrc;
		default:
			var_dump($this->fetchType);
			assert(0);
		}
	}
}
// }}}
class Decompiler_Box // {{{
{
	var $obj;

	function Decompiler_Box(&$obj)
	{
		$this->obj = &$obj;
	}

	function __toString()
	{
		return $this->obj->__toString();
	}
}
// }}}
class Decompiler_Dim extends Decompiler_Value // {{{
{
	var $offsets = array();
	var $isLast = false;
	var $assign = null;

	function __toString()
	{
		if (is_a($this->value, 'Decompiler_ListBox')) {
			$exp = str($this->value->obj->src);
		}
		else {
			$exp = str($this->value);
		}
		foreach ($this->offsets as $dim) {
			$exp .= '[' . str($dim) . ']';
		}
		return $exp;
	}
}
// }}}
class Decompiler_DimBox extends Decompiler_Box // {{{
{
}
// }}}
class Decompiler_List extends Decompiler_Code // {{{
{
	var $src;
	var $dims = array();
	var $everLocked = false;

	function __toString()
	{
		if (count($this->dims) == 1 && !$this->everLocked) {
			$dim = $this->dims[0];
			unset($dim->value);
			$dim->value = $this->src;
			if (!isset($dim->assign)) {
				return str($dim);
			}
			return str($this->dims[0]->assign) . ' = ' . str($dim);
		}
		/* flatten dims */
		$assigns = array();
		foreach ($this->dims as $dim) {
			$assign = &$assigns;
			foreach ($dim->offsets as $offset) {
				$assign = &$assign[$offset];
			}
			$assign = str($dim->assign);
		}
		return $this->toList($assigns) . ' = ' . str($this->src);
	}

	function toList($assigns)
	{
		$keys = array_keys($assigns);
		if (count($keys) < 2) {
			$keys[] = 0;
		}
		$max = call_user_func_array('max', $keys);
		$list = 'list(';
		for ($i = 0; $i <= $max; $i ++) {
			if ($i) {
				$list .= ', ';
			}
			if (!isset($assigns[$i])) {
				continue;
			}
			if (is_array($assigns[$i])) {
				$list .= $this->toList($assigns[$i]);
			}
			else {
				$list .= $assigns[$i];
			}
		}
		return $list . ')';
	}
}
// }}}
class Decompiler_ListBox extends Decompiler_Box // {{{
{
}
// }}}
class Decompiler_Array extends Decompiler_Value // {{{
{
	var $needExport = false;
	var $indent = '';

	function Decompiler_Array($value = array(), $needexport = false, $indent = '')
	{
		$this->value = $value;
		$this->needExport = $needexport;
		$this->indent = $indent;
	}

	function __toString()
	{
		$exp = "array(";
		$indent = $this->indent . INDENT;
		$assoclen = 0;
		$multiline = 0;
		$i = 0;
		foreach ($this->value as $k => $v) {
			if ($i !== $k) {
				$len = strlen($k);
				if ($assoclen < $len) {
					$assoclen = $len;
				}
			}
			if (is_array($v)) {
				$multiline ++;
			}
			++ $i;
		}
		if ($assoclen && $this->needExport) {
			$assoclen += 2;
		}

		$i = 0;
		$subindent = $indent . INDENT;
		foreach ($this->value as $k => $v) {
			if ($multiline) {
				if ($i) {
					$exp .= ",";
				}
				$exp .= "\n";
				$exp .= $indent;
			}
			else {
				if ($i) {
					$exp .= ", ";
				}
			}

			if ($this->needExport) {
				$k = var_export($k, true);
			}
			if ($multiline) {
				$exp .= sprintf("%{$assoclen}s => ", $k);
			}
			else if ($assoclen) {
				$exp .= $k . ' => ';
			}

			if (is_array($v)) {
				$v = new Decompiler_Array($v, $this->needExport);
			}
			$exp .= str($v, $subindent);

			$i ++;
		}
		if ($multiline) {
			$exp .= "$indent);";
		}
		else {
			$exp .= ")";
		}
		return $exp;
	}
}
// }}}
class Decompiler_ForeachBox extends Decompiler_Box // {{{
{
	var $iskey;

	function __toString()
	{
		return 'foreach (' . '';
	}
}
// }}}

class Decompiler
{
	var $rName = '!^[\\w_][\\w\\d_]*$!';
	var $rQuotedName = "!^'[\\w_][\\w\\d_]*'\$!";

	function Decompiler()
	{
		// {{{ opinfo
		$this->unaryops = array(
				XC_BW_NOT   => '~',
				XC_BOOL_NOT => '!',
				);
		$this->binops = array(
				XC_ADD                 => "+",
				XC_ASSIGN_ADD          => "+=",
				XC_SUB                 => "-",
				XC_ASSIGN_SUB          => "-=",
				XC_MUL                 => "*",
				XC_ASSIGN_MUL          => "*=",
				XC_DIV                 => "/",
				XC_ASSIGN_DIV          => "/=",
				XC_MOD                 => "%",
				XC_ASSIGN_MOD          => "%=",
				XC_SL                  => "<<",
				XC_ASSIGN_SL           => "<<=",
				XC_SR                  => ">>",
				XC_ASSIGN_SR           => ">>=",
				XC_CONCAT              => ".",
				XC_ASSIGN_CONCAT       => ".=",
				XC_IS_IDENTICAL        => "===",
				XC_IS_NOT_IDENTICAL    => "!==",
				XC_IS_EQUAL            => "==",
				XC_IS_NOT_EQUAL        => "!=",
				XC_IS_SMALLER          => "<",
				XC_IS_SMALLER_OR_EQUAL => "<=",
				XC_BW_OR               => "|",
				XC_ASSIGN_BW_OR        => "|=",
				XC_BW_AND              => "&",
				XC_ASSIGN_BW_AND       => "&=",
				XC_BW_XOR              => "^",
				XC_ASSIGN_BW_XOR       => "^=",
				XC_BOOL_XOR            => "xor",
				);
		// }}}
		$this->includeTypes = array( // {{{
				ZEND_EVAL         => 'eval',
				ZEND_INCLUDE      => 'include',
				ZEND_INCLUDE_ONCE => 'include_once',
				ZEND_REQUIRE      => 'require',
				ZEND_REQUIRE_ONCE => 'require_once',
				);
				// }}}
	}
	function outputPhp(&$opcodes, $opline, $last, $indent) // {{{
	{
		$origindent = $indent;
		$curticks = 0;
		for ($i = $opline; $i <= $last; $i ++) {
			$op = $opcodes[$i];
			if (isset($op['php'])) {
				$toticks = isset($op['ticks']) ? $op['ticks'] : 0;
				if ($curticks != $toticks) {
					if (!$toticks) {
						echo $origindent, "}\n";
						$indent = $origindent;
					}
					else {
						if ($curticks) {
							echo $origindent, "}\n";
						}
						else if (!$curticks) {
							$indent .= INDENT;
						}
						echo $origindent, "declare(ticks=$curticks) {\n";
					}
					$curticks = $toticks;
				}
				echo $indent, str($op['php']), ";\n";
			}
		}
		if ($curticks) {
			echo $origindent, "}\n";
		}
	}
	// }}}
	function getOpVal($op, &$EX, $tostr = true, $free = false) // {{{
	{
		switch ($op['op_type']) {
		case XC_IS_CONST:
			return str(value($op['u.constant']));

		case XC_IS_VAR:
		case XC_IS_TMP_VAR:
			$T = &$EX['Ts'];
			$ret = $T[$op['u.var']];
			if ($tostr) {
				$ret = str($ret, $EX);
			}
			if ($free) {
				unset($T[$op['u.var']]);
			}
			return $ret;

		case XC_IS_CV:
			$var = $op['u.var'];
			$var = $EX['op_array']['vars'][$var];
			return '$' . $var['name'];

		case XC_IS_UNUSED:
			return null;
		}
	}
	// }}}
	function &dop_array($op_array, $indent = '') // {{{
	{
		$opcodes = &$op_array['opcodes'];
		$last = count($opcodes) - 1;
		if ($opcodes[$last]['opcode'] == XC_HANDLE_EXCEPTION) {
			unset($opcodes[$last]);
		}
		$EX['indent'] = '';
		//for ($i = 0, $cnt = count($opcodes); $i < $cnt; $i ++) {
		//	$opcodes[$i]['opcode'] = xcache_get_fixed_opcode($opcodes[$i]['opcode'], $i);
		//}
		// {{{ build jmp array
		for ($i = 0, $cnt = count($opcodes); $i < $cnt; $i ++) {
			$op = &$opcodes[$i];
			/*
			if ($op['opcode'] == XC_JMPZ) {
				$this->dumpop($op, $EX);
				var_dump($op);
			}
			continue;
			*/
			$op['line'] = $i;
			switch ($op['opcode']) {
			case XC_JMP:
				$target = $op['op1']['u.var'];
				$op['jmpouts'] = array($target);
				$opcodes[$target]['jmpins'][] = $i;
				break;

			case XC_JMPZNZ:
				$jmpz = $op['op2']['u.opline_num'];
				$jmpnz = $op['extended_value'];
				$op['jmpouts'] = array($jmpz, $jmpnz);
				$opcodes[$jmpz]['jmpins'][] = $i;
				$opcodes[$jmpnz]['jmpins'][] = $i;
				break;

			case XC_JMPZ:
			case XC_JMPNZ:
			case XC_JMPZ_EX:
			case XC_JMPNZ_EX:
			// case XC_FE_RESET:
			case XC_FE_FETCH:
			// case XC_JMP_NO_CTOR:
				$target = $op['op2']['u.opline_num'];
				//if (!isset($target)) {
				//	$this->dumpop($op, $EX);
				//	var_dump($op); exit;
				//}
				$op['jmpouts'] = array($target);
				$opcodes[$target]['jmpins'][] = $i;
				break;

			/*
			case XC_RETURN:
				$op['jmpouts'] = array();
				break;
			*/
			}
		}
		unset($op);
		// }}}
		// build semi-basic blocks
		$nextbbs = array();
		$starti = 0;
		for ($i = 1, $cnt = count($opcodes); $i < $cnt; $i ++) {
			if (isset($opcodes[$i]['jmpins'])
			 || isset($opcodes[$i - 1]['jmpouts'])) {
				$nextbbs[$starti] = $i;
				$starti = $i;
			}
		}
		$nextbbs[$starti] = $cnt;

		$EX = array();
		$EX['Ts'] = array();
		$EX['indent'] = $indent;
		$EX['nextbbs'] = $nextbbs;
		$EX['op_array'] = &$op_array;
		$EX['opcodes'] = &$opcodes;
		// func call
		$EX['object'] = null;
		$EX['fbc'] = null;
		$EX['argstack'] = array();
		$EX['arg_types_stack'] = array();
		$EX['last'] = count($opcodes) - 1;
		$EX['silence'] = 0;

		for ($next = 0, $last = $EX['last'];
				$loop = $this->outputCode($EX, $next, $last, $indent, true);
				list($next, $last) = $loop) {
			// empty
		}
		return $EX;
	}
	// }}}
	function outputCode(&$EX, $opline, $last, $indent, $loop = false) // {{{
	{
		$op = &$EX['opcodes'][$opline];
		$next = $EX['nextbbs'][$opline];

		$end = $next - 1;
		if ($end > $last) {
			$end = $last;
		}

		if (isset($op['jmpins'])) {
			echo "\nline", $op['line'], ":\n";
		}
		else {
			// echo ";;;\n";
		}
		$this->dasmBasicBlock($EX, $opline, $end);
		$this->outputPhp($EX['opcodes'], $opline, $end, $indent);
		// jmpout op
		$op = &$EX['opcodes'][$end];
		$op1 = $op['op1'];
		$op2 = $op['op2'];
		$ext = $op['extended_value'];
		$line = $op['line'];

		if (isset($EX['opcodes'][$next])) {
			if (isset($last) && $next > $last) {
				$next = null;
			}
		}
		else {
			$next = null;
		}
		if ($op['opcode'] == XC_FE_FETCH) {
			$opline = $next;
			$next = $op['op2']['u.opline_num'];
			$end = $next - 1;

			ob_start();
			$this->outputCode($EX, $opline, $end /* - 1 skip last jmp */, $indent . INDENT);
			$body = ob_get_clean();

			$as = str($op['fe_as']);
			if (isset($op['fe_key'])) {
				$as = str($op['fe_key']) . ' => ' . $as;
			}
			echo "{$indent}foreach (" . str($op['fe_src']) . " as $as) {\n";
			echo $body;
			echo "{$indent}}";
			// $this->outputCode($EX, $next, $last, $indent);
			// return;
		}
		/*
		if ($op['opcode'] == XC_JMPZ) {
			$target = $op2['u.opline_num'];
			if ($line + 1) {
				$nextblock = $EX['nextbbs'][$next];
				$jmpop = end($nextblock);
				if ($jmpop['opcode'] == XC_JMP) {
					$ifendline = $op2['u.opline_num'];
					if ($ifendline >= $line) {
						$cond = $op['cond'];
						echo "{$indent}if ($cond) {\n";
						$this->outputCode($EX, $next, $last, INDENT . $indent);
						echo "$indent}\n";
						$this->outputCode($EX, $target, $last, $indent);
						return;
					}
				}
			}
		}
		*/
		if (!isset($next)) {
			return;
		}
		if (!empty($op['jmpouts']) && isset($op['isjmp'])) {
			if (isset($op['cond'])) {
				echo "{$indent}check ($op[cond]) {\n";
				echo INDENT;
			}
			echo $indent;
			echo xcache_get_opcode($op['opcode']), ' line', $op['jmpouts'][0];
			if (isset($op['jmpouts'][1])) {
				echo ', line', $op['jmpouts'][1];
			}
			echo ";";
			// echo ' // <- line', $op['line'];
			echo "\n";
			if (isset($op['cond'])) echo "$indent}\n";
		}

		// proces JMPZ_EX/JMPNZ_EX for AND,OR
		$op = &$EX['opcodes'][$next];
		/*
		if (isset($op['jmpins'])) {
			foreach (array_reverse($op['jmpins']) as $fromline) {
				$fromop = $EX['opcodes'][$fromline];
				switch ($fromop['opcode']) {
				case XC_JMPZ_EX: $opstr = 'and'; break;
				case XC_JMPNZ_EX: $opstr = 'or'; break;
				case XC_JMPZNZ: var_dump($fromop); exit;
				default: continue 2;
				}

				$var = $fromop['result']['u.var'];
				var_dump($EX['Ts'][$var]);
				$EX['Ts'][$var] = '(' . $fromop['and_or'] . " $opstr " . $EX['Ts'][$var] . ')';
			}
			#$this->outputCode($EX, $next, $last, $indent);
			#return;
		}
		*/
		if (isset($op['cond_false'])) {
			// $this->dumpop($op, $EX);
			// any true comes here, so it's a "or"
			$cond = implode(' and ', $op['cond_false']);
			// var_dump($op['cond'] = $cond);
			/*
			$rvalue = implode(' or ', $op['cond_true']) . ' or ' . $rvalue;
			unset($op['cond_true']);
			*/
		}

		if ($loop) {
			return array($next, $last);
		}
		$this->outputCode($EX, $next, $last, $indent);
	}
	// }}}
	function unquoteName($str) // {{{
	{
		if (preg_match($this->rQuotedName, $str)) {
			$str = substr($str, 1, -1);
		}
		return $str;
	}
	// }}}
	function dasmBasicBlock(&$EX, $opline, $last) // {{{
	{
		$T = &$EX['Ts'];
		$opcodes = &$EX['opcodes'];
		$lastphpop = null;

		for ($i = $opline, $ic = $last + 1; $i < $ic; $i ++) {
			// {{{ prepair
			$op = &$opcodes[$i];
			$opc = $op['opcode'];
			if ($opc == XC_NOP) {
				continue;
			}

			$op1 = $op['op1'];
			$op2 = $op['op2'];
			$res = $op['result'];
			$ext = $op['extended_value'];

			$opname = xcache_get_opcode($opc);

			if ($opname == 'UNDEF' || !isset($opname)) {
				echo 'UNDEF OP:';
				$this->dumpop($op, $EX);
				continue;
			}
			// $this->dumpop($op, $EX); //var_dump($op);

			$resvar = null;
			if (($res['u.EA.type'] & EXT_TYPE_UNUSED) || $res['op_type'] == XC_IS_UNUSED) {
				$istmpres = false;
			}
			else {
				$istmpres = true;
			}
			// }}}
			// echo $opname, "\n";

			$call = array(&$this, $opname);
			if (is_callable($call)) {
				$this->{$opname}($op, $EX);
			}
			else if (isset($this->binops[$opc])) { // {{{
				$op1val = $this->getOpVal($op1, $EX, false);
				$op2val = $this->getOpVal($op2, $EX, false);
				$rvalue = new Decompiler_Binop($this, $op1val, $opc, $op2val);
				$resvar = $rvalue;
				// }}}
			}
			else if (isset($this->unaryops[$opc])) { // {{{
				$op1val = $this->getOpVal($op1, $EX);
				$myop = $this->unaryops[$opc];
				$rvalue = "$myop$op1val";
				$resvar = $rvalue;
				// }}}
			}
			else {
				switch ($opc) {
				case XC_NEW: // {{{
					array_push($EX['arg_types_stack'], array($EX['object'], $EX['fbc']));
					$EX['object'] = (int) $res['u.var'];
					$EX['fbc'] = 'new ' . $this->unquoteName($this->getOpVal($op1, $EX));
					if (PHP_VERSION < 5) {
						$resvar = '$new object$';
					}
					break;
					// }}}
				case XC_FETCH_CLASS: // {{{
					if ($op2['op_type'] == XC_IS_UNUSED) {
						switch ($ext) {
						case ZEND_FETCH_CLASS_SELF:
							$class = 'self';
							break;
						case ZEND_FETCH_CLASS_PARENT:
							$class = 'parent';
						}
					}
					else {
						$class = $op2['u.constant'];
						if (is_object($class)) {
							$class = get_class($class);
						}
					}
					$resvar = $class;
					break;
					// }}}
				case XC_FETCH_CONSTANT: // {{{
					if ($op1['op_type'] == XC_IS_CONST) {
						$resvar = $op1['u.constant'];
					}
					else if ($op1['op_type'] == XC_IS_UNUSED) {
						$resvar = $op2['u.constant'];
					}
					else {
						$class = $T[$op1['u.var']];
						assert($class[0] == 'class');
						$resvar = $class[1] . '::' . $op2['u.constant'];
					}
					break;
					// }}}
					// {{{ case XC_FETCH_*
				case XC_FETCH_R:
				case XC_FETCH_W:
				case XC_FETCH_RW:
				case XC_FETCH_FUNC_ARG:
				case XC_FETCH_UNSET:
				case XC_FETCH_IS:
				case XC_UNSET_VAR:
					$rvalue = $this->getOpVal($op1, $EX);
					$fetchtype = $op2[PHP_VERSION < 5 ? 'u.fetch_type' : 'u.EA.type'];
					switch ($fetchtype) {
					case ZEND_FETCH_STATIC_MEMBER:
						$class = $this->getOpVal($op2, $EX);
						$rvalue = $class . '::$' . $this->unquoteName($rvalue);
						break;
					default:
						$name = $this->unquoteName($rvalue);
						$globalname = xcache_is_autoglobal($name) ? "\$$name" : "\$GLOBALS[$rvalue]";
						$rvalue = new Decompiler_Fetch($rvalue, $fetchtype, $globalname);
						break;
					}
					if ($opc == XC_UNSET_VAR) {
						$op['php'] = "unset(" . str($rvalue) . ")";
						$lastphpop = &$op;
					}
					else if ($res['op_type'] != XC_IS_UNUSED) {
						$resvar = $rvalue;
					}
					break;
					// }}}
					// {{{ case XC_FETCH_DIM_*
				case XC_FETCH_DIM_TMP_VAR:
				case XC_FETCH_DIM_R:
				case XC_FETCH_DIM_W:
				case XC_FETCH_DIM_RW:
				case XC_FETCH_DIM_FUNC_ARG:
				case XC_FETCH_DIM_UNSET:
				case XC_FETCH_DIM_IS:
				case XC_ASSIGN_DIM:
				case XC_UNSET_DIM:
				case XC_UNSET_DIM_OBJ:
					$src = $this->getOpVal($op1, $EX, false);
					if (is_a($src, "Decompiler_ForeachBox")) {
						$src->iskey = $this->getOpVal($op2, $EX);
						$resvar = $src;
						break;
					}
					else if (is_a($src, "Decompiler_DimBox")) {
						$dimbox = $src;
					}
					else {
						if (!is_a($src, "Decompiler_ListBox")) {
							$list = new Decompiler_List($this->getOpVal($op1, $EX, false));

							$src = new Decompiler_ListBox($list);
							if (!isset($op1['u.var'])) {
								$this->dumpop($op, $EX);
								var_dump($op);
								die('missing u.var');
							}
							$T[$op1['u.var']] = $src;
							unset($list);
						}
						$dim = new Decompiler_Dim($src);
						$src->obj->dims[] = &$dim;

						$dimbox = new Decompiler_DimBox($dim);
					}
					$dim = &$dimbox->obj;
					$dim->offsets[] = $this->getOpVal($op2, $EX);
					if ($ext == ZEND_FETCH_ADD_LOCK) {
						$src->obj->everLocked = true;
					}
					else if ($ext == ZEND_FETCH_STANDARD) {
						$dim->isLast = true;
					}
					unset($dim);
					$rvalue = $dimbox;

					if ($opc == XC_ASSIGN_DIM) {
						$lvalue = $rvalue;
						++ $i;
						$rvalue = $this->getOpVal($opcodes[$i]['op1'], $EX);
						$resvar = str($lvalue) . ' = ' . $rvalue;
					}
					else if ($opc == XC_UNSET_DIM) {
						$op['php'] = "unset(" . str($rvalue) . ")";
						$lastphpop = &$op;
					}
					else if ($res['op_type'] != XC_IS_UNUSED) {
						$resvar = $rvalue;
					}
					break;
					// }}}
				case XC_ASSIGN: // {{{
					$lvalue = $this->getOpVal($op1, $EX);
					$rvalue = $this->getOpVal($op2, $EX, false);
					if (is_a($rvalue, 'Decompiler_ForeachBox')) {
						$type = $rvalue->iskey ? 'fe_key' : 'fe_as';
						$rvalue->obj[$type] = $lvalue;
						unset($T[$op2['u.var']]);
						break;
					}
					if (is_a($rvalue, "Decompiler_DimBox")) {
						$dim = &$rvalue->obj;
						$dim->assign = $lvalue;
						if ($dim->isLast) {
							$resvar = str($dim->value);
						}
						unset($dim);
						break;
					}
					$resvar = "$lvalue = " . str($rvalue, $EX);
					break;
					// }}}
				case XC_ASSIGN_REF: // {{{
					$lvalue = $this->getOpVal($op1, $EX);
					$rvalue = $this->getOpVal($op2, $EX, false);
					if (is_a($rvalue, 'Decompiler_Fetch')) {
						$src = str($rvalue->src);
						if (substr($src, 1, -1) == substr($lvalue, 1)) {
							switch ($rvalue->fetchType) {
							case ZEND_FETCH_GLOBAL:
								$resvar = 'global ' . $lvalue;
								break 2;
							case ZEND_FETCH_STATIC:
								$statics = &$EX['op_array']['static_variables'];
								$resvar = 'static ' . $lvalue;
								$name = substr($src, 1, -1);
								if (isset($statics[$name])) {
									$var = $statics[$name];
									$resvar .= ' = ';
									$resvar .= str(value($var), $EX);
								}
								unset($statics);
								break 2;
							}
						}
					}
					$rvalue = str($rvalue);
					$resvar = "$lvalue = &$rvalue";
					break;
					// }}}
				// {{{ case XC_FETCH_OBJ_*
				case XC_FETCH_OBJ_R:
				case XC_FETCH_OBJ_W:
				case XC_FETCH_OBJ_RW:
				case XC_FETCH_OBJ_FUNC_ARG:
				case XC_FETCH_OBJ_UNSET:
				case XC_FETCH_OBJ_IS:
				case XC_ASSIGN_OBJ:
					$obj = $this->getOpVal($op1, $EX);
					if (!isset($obj)) {
						$obj = '$this';
					}
					$prop = $this->getOpVal($op2, $EX);
					if (preg_match($this->rQuotedName, $prop)) {
						$prop = substr($prop, 1, -1);;
						$rvalue = "{$obj}->$prop";
					}
					else {
						$rvalue = "{$obj}->{" . "$prop}";
					}
					if ($res['op_type'] != XC_IS_UNUSED) {
						$resvar = $rvalue;
					}
					if ($opc == XC_ASSIGN_OBJ) {
						++ $i;
						$lvalue = $rvalue;
						$rvalue = $this->getOpVal($opcodes[$i]['op1'], $EX);
						$resvar = "$lvalue = $rvalue";
					}
					break;
					// }}}
				case XC_ISSET_ISEMPTY_DIM_OBJ:
				case XC_ISSET_ISEMPTY_PROP_OBJ:
				case XC_ISSET_ISEMPTY:
				case XC_ISSET_ISEMPTY_VAR: // {{{
					if ($opc == XC_ISSET_ISEMPTY_VAR) {
						$rvalue = $this->getOpVal($op1, $EX);;
						if (preg_match($this->rQuotedName, $rvalue)) {
							$rvalue = '$' . substr($rvalue, 1, -1);
						}
						else {
							$rvalue = "${" . $rvalue . "}";
						}
						if ($op2['u.EA.type'] == ZEND_FETCH_STATIC_MEMBER) {
							$class = $this->getOpVal($op2, $EX);
							$rvalue = $class . '::' . $rvalue;
						}
					}
					else if ($opc == XC_ISSET_ISEMPTY) {
						$rvalue = $this->getOpVal($op1, $EX);
					}
					else {
						$container = $this->getOpVal($op1, $EX);
						$dim = $this->getOpVal($op2, $EX);
						$rvalue = $container . "[$dim]";
					}

					switch (PHP_VERSION < 5 ? $op['op2']['u.var'] /* u.constant */ : $ext) {
					case ZEND_ISSET:
						$rvalue = "isset($rvalue)";
						break;
					case ZEND_ISEMPTY:
						$rvalue = "empty($rvalue)";
						break;
					default:
						$this->dumpop($op, $EX);
						die_error('1');
					}
					$resvar = $rvalue;
					break;
					// }}}
				case XC_SEND_VAR_NO_REF:
				case XC_SEND_VAL:
				case XC_SEND_REF:
				case XC_SEND_VAR: // {{{
					$ref = ($opc == XC_SEND_REF ? '&' : '');
					$EX['argstack'][] = $ref . $this->getOpVal($op1, $EX);
					break;
					// }}}
				case XC_INIT_METHOD_CALL:
				case XC_INIT_FCALL_BY_FUNC:
				case XC_INIT_FCALL_BY_NAME: // {{{
					if (($ext & ZEND_CTOR_CALL)) {
						break;
					}
					array_push($EX['arg_types_stack'], array($EX['object'], $EX['fbc']));
					if ($opc == XC_INIT_METHOD_CALL || $op1['op_type'] != XC_IS_UNUSED) {
						$obj = $this->getOpVal($op1, $EX);
						if (!isset($obj)) {
							$obj = '$this';
						}
						$EX['object'] = $obj;
						if ($res['op_type'] != XC_IS_UNUSED) {
							$resvar = '$obj call$';
						}
					}
					else {
						$EX['object'] = null;
					}

					if ($opc == XC_INIT_FCALL_BY_FUNC) {
						$which = $op1['u.var'];
						$EX['fbc'] = $EX['op_array']['funcs'][$which]['name'];
					}
					else {
						$EX['fbc'] = $this->getOpVal($op2, $EX, false);
					}
					break;
					// }}}
				case XC_DO_FCALL_BY_FUNC:
					$which = $op1['u.var'];
					$fname = $EX['op_array']['funcs'][$which]['name'];
					$args = $this->popargs($EX, $ext);
					$resvar = $fname . "($args)";
					break;
				case XC_DO_FCALL:
					$fname = $this->unquoteName($this->getOpVal($op1, $EX, false));
					$args = $this->popargs($EX, $ext);
					$resvar = $fname . "($args)";
					break;
				case XC_DO_FCALL_BY_NAME: // {{{
					$object = null;

					$fname = $this->unquoteName($EX['fbc']);
					if (!is_int($EX['object'])) {
						$object = $EX['object'];
					}

					$args = $this->popargs($EX, $ext);

					$resvar =
						(isset($object) ? $object . '->' : '' )
						. $fname . "($args)";
					unset($args);

					if (is_int($EX['object'])) {
						$T[$EX['object']] = $resvar;
						$resvar = null;
					}
					list($EX['object'], $EX['fbc']) = array_pop($EX['arg_types_stack']);
					break;
					// }}}
				case XC_VERIFY_ABSTRACT_CLASS: // {{{
					//unset($T[$op1['u.var']]);
					break;
					// }}}
				case XC_DECLARE_CLASS: 
				case XC_DECLARE_INHERITED_CLASS: // {{{
					$key = $op1['u.constant'];
					$class = &$this->dc['class_table'][$key];
					if (!isset($class)) {
						echo 'class not found: ' . $key;
						exit;
					}
					$class['name'] = $this->unquoteName($this->getOpVal($op2, $EX));
					if ($opc == XC_DECLARE_INHERITED_CLASS) {
						$ext /= XC_SIZEOF_TEMP_VARIABLE;
						$class['parent'] = $T[$ext];
						unset($T[$ext]);
					}
					else {
						$class['parent'] = null;
					}

					while ($i + 2 < $ic
					 && $opcodes[$i + 2]['opcode'] == XC_ADD_INTERFACE
					 && $opcodes[$i + 2]['op1']['u.var'] == $res['u.var']
					 && $opcodes[$i + 1]['opcode'] == XC_FETCH_CLASS) {
						$fetchop = &$opcodes[$i + 1];
						$impl = $this->unquoteName($this->getOpVal($fetchop['op2'], $EX));
						$addop = &$opcodes[$i + 2];
						$class['interfaces'][$addop['extended_value']] = $impl;
						unset($fetchop, $addop);
						$i += 2;
					}
					$this->dclass($class);
					unset($class);
					break;
					// }}}
				case XC_INIT_STRING: // {{{
					$resvar = "''";
					break;
					// }}}
				case XC_ADD_CHAR:
				case XC_ADD_STRING:
				case XC_ADD_VAR: // {{{
					$op1val = $this->getOpVal($op1, $EX);
					$op2val = $this->getOpVal($op2, $EX);
					switch ($opc) {
					case XC_ADD_CHAR:
						$op2val = str(chr($op2val), $EX);
						break;
					case XC_ADD_STRING:
						$op2val = str($op2val, $EX);
						break;
					case XC_ADD_VAR:
						break;
					}
					if ($op1val == "''") {
						$rvalue = $op2val;
					}
					else if ($op2val == "''") {
						$rvalue = $op1val;
					}
					else {
						$rvalue = $op1val . ' . ' . $op2val;
					}
					$resvar = $rvalue;
					// }}}
					break;
				case XC_PRINT: // {{{
					$op1val = $this->getOpVal($op1, $EX);
					$resvar = "print($op1val)";
					break;
					// }}}
				case XC_ECHO: // {{{
					$op1val = $this->getOpVal($op1, $EX);
					$resvar = "echo $op1val";
					break;
					// }}}
				case XC_EXIT: // {{{
					$op1val = $this->getOpVal($op1, $EX);
					$resvar = "exit($op1val)";
					break;
					// }}}
				case XC_INIT_ARRAY:
				case XC_ADD_ARRAY_ELEMENT: // {{{
					$rvalue = $this->getOpVal($op1, $EX, false, true);

					if ($opc == XC_ADD_ARRAY_ELEMENT) {
						$offset = $this->getOpVal($op2, $EX);
						if (isset($offset)) {
							$T[$res['u.var']]->value[$offset] = $rvalue;
						}
						else {
							$T[$res['u.var']]->value[] = $rvalue;
						}
					}
					else {
						if ($opc == XC_INIT_ARRAY) {
							$resvar = new Decompiler_Array();
							if (!isset($rvalue)) {
								continue;
							}
						}

						$offset = $this->getOpVal($op2, $EX);
						if (isset($offset)) {
							$resvar->value[$offset] = $rvalue;
						}
						else {
							$resvar->value[] = $rvalue;
						}
					}
					break;
					// }}}
				case XC_QM_ASSIGN: // {{{
					$resvar = $this->getOpVal($op1, $EX);
					break;
					// }}}
				case XC_BOOL: // {{{
					$resvar = /*'(bool) ' .*/ $this->getOpVal($op1, $EX);
					break;
					// }}}
				case XC_RETURN: // {{{
					$resvar = "return " . $this->getOpVal($op1, $EX);
					break;
					// }}}
				case XC_INCLUDE_OR_EVAL: // {{{
					$type = $op2['u.var']; // hack
					$keyword = $this->includeTypes[$type];
					$resvar = "$keyword(" . $this->getOpVal($op1, $EX) . ")";
					break;
					// }}}
				case XC_FE_RESET: // {{{
					$resvar = $this->getOpVal($op1, $EX);
					break;
					// }}}
				case XC_FE_FETCH: // {{{
					$op['fe_src'] = $this->getOpVal($op1, $EX);
					$fe = new Decompiler_ForeachBox($op);
					$fe->iskey = false;
					$T[$res['u.var']] = $fe;

					++ $i;
					if (($ext & ZEND_FE_FETCH_WITH_KEY)) {
						$fe = new Decompiler_ForeachBox($op);
						$fe->iskey = true;

						$res = $opcodes[$i]['result'];
						$T[$res['u.var']] = $fe;
					}
					break;
					// }}}
				case XC_SWITCH_FREE: // {{{
					// unset($T[$op1['u.var']]);
					break;
					// }}}
				case XC_FREE: // {{{
					$free = $T[$op1['u.var']];
					if (!is_a($free, 'Decompiler_Array') && !is_a($free, 'Decompiler_Box')) {
						$op['php'] = is_object($free) ? $free : $this->unquote($free, '(', ')');
						$lastphpop = &$op;
					}
					unset($T[$op1['u.var']], $free);
					break;
					// }}}
				case XC_JMP_NO_CTOR:
					break;
				case XC_JMPNZ: // while
				case XC_JMPZNZ: // for
				case XC_JMPZ_EX: // and
				case XC_JMPNZ_EX: // or
				case XC_JMPZ: // {{{
					if ($opc == XC_JMP_NO_CTOR && $EX['object']) {
						$rvalue = $EX['object'];
					}
					else {
						$rvalue = $this->getOpVal($op1, $EX);
					}

					if (isset($op['cond_true'])) {
						// any true comes here, so it's a "or"
						$rvalue = implode(' or ', $op['cond_true']) . ' or ' . $rvalue;
						unset($op['cond_true']);
					}
					if (isset($op['cond_false'])) {
						var_dump($op);// exit;
					}
					if ($opc == XC_JMPZ_EX || $opc == XC_JMPNZ_EX || $opc == XC_JMPZ) {
						$targetop = &$EX['opcodes'][$op2['u.opline_num']];
						if ($opc == XC_JMPNZ_EX) {
							$targetop['cond_true'][] = str($rvalue);
						}
						else {
							$targetop['cond_false'][] = str($rvalue);
						}
						unset($targetop);
					}
					else {
						$op['cond'] = $rvalue; 
						$op['isjmp'] = true;
					}
					break;
					// }}}
				case XC_JMP: // {{{
					$op['cond'] = null;
					$op['isjmp'] = true;
					break;
					// }}}
				case XC_CASE:
				case XC_BRK:
					break;
				case XC_RECV_INIT:
				case XC_RECV:
					$offset = $this->getOpVal($op1, $EX);
					$lvalue = $this->getOpVal($op['result'], $EX);
					if ($opc == XC_RECV_INIT) {
						$default = value($op['op2']['u.constant']);
					}
					else {
						$default = null;
					}
					$EX['recvs'][$offset] = array($lvalue, $default);
					break;
				case XC_POST_DEC:
				case XC_POST_INC:
				case XC_POST_DEC_OBJ:
				case XC_POST_INC_OBJ:
				case XC_PRE_DEC:
				case XC_PRE_INC:
				case XC_PRE_DEC_OBJ:
				case XC_PRE_INC_OBJ: // {{{
					$flags = array_flip(explode('_', $opname));
					if (isset($flags['OBJ'])) {
						$resvar = $this->getOpVal($op1, $EX);
						$prop = $this->unquoteName($this->getOpVal($op2, $EX));
						if ($prop{0} == '$') {
							$resvar = $resvar . "{" . $prop . "}";
						}
						else {
							$resvar = $resvar . "->" . $prop;
						}
					}
					else {
						$resvar = $this->getOpVal($op1, $EX);
					}
					$opstr = isset($flags['DEC']) ? '--' : '++';
					if (isset($flags['POST'])) {
						$resvar .= ' ' . $opstr;
					}
					else {
						$resvar = "$opstr $resvar";
					}
					break;
					// }}}

				case XC_BEGIN_SILENCE: // {{{
					$EX['silence'] ++;
					break;
					// }}}
				case XC_END_SILENCE: // {{{
					$EX['silence'] --;
					$lastresvar = '@' . str($lastresvar);
					break;
					// }}}
				case XC_CONT: // {{{
					break;
					// }}}
				case XC_CAST: // {{{
					$type = $ext;
					static $type2cast = array(
							IS_LONG   => '(int)',
							IS_DOUBLE => '(double)',
							IS_STRING => '(string)',
							IS_ARRAY  => '(array)',
							IS_OBJECT => '(object)',
							IS_BOOL   => '(bool)',
							IS_NULL   => '(unset)',
							);
					assert(isset($type2cast[$type]));
					$cast = $type2cast[$type];
					$resvar = $cast . ' ' . $this->getOpVal($op1, $EX);
					break;
					// }}}
				case XC_EXT_STMT:
				case XC_EXT_FCALL_BEGIN:
				case XC_EXT_FCALL_END:
				case XC_EXT_NOP:
					break;
				case XC_DECLARE_FUNCTION_OR_CLASS:
					/* always removed by compiler */
					break;
				case XC_TICKS:
					$lastphpop['ticks'] = $this->getOpVal($op1, $EX);
					// $EX['tickschanged'] = true;
					break;
				default: // {{{
					echo "\x1B[31m * TODO ", $opname, "\x1B[0m\n";
					// }}}
				}
			}
			if (isset($resvar)) {
				if ($istmpres) {
					$T[$res['u.var']] = $resvar;
					$lastresvar = &$T[$res['u.var']];
				}
				else {
					$op['php'] = $resvar;
					$lastphpop = &$op;
					$lastresvar = &$op['php'];
				}
			}
		}
		return $T;
	}
	// }}}
	function unquote($str, $st, $ed) // {{{
	{
		$l1 = strlen($st);
		$l2 = strlen($ed);
		if (substr($str, 0, $l1) === $st && substr($str, -$l2) === $ed) {
			$str = substr($str, $l1, -$l2);
		}
		return $str;
	}
	// }}}
	function popargs(&$EX, $n) // {{{
	{
		$args = array();
		for ($i = 0; $i < $n; $i ++) {
			$a = array_pop($EX['argstack']);
			if (is_array($a)) {
				array_unshift($args, str($a, $EX));
			}
			else {
				array_unshift($args, $a);
			}
		}
		return implode(', ', $args);
	}
	// }}}
	function dumpop($op, &$EX) // {{{
	{
		$op1 = $op['op1'];
		$op2 = $op['op2'];
		$d = array('opname' => xcache_get_opcode($op['opcode']), 'opcode' => $op['opcode']);

		foreach (array('op1' => 'op1', 'op2' => 'op2', 'result' => 'res') as $k => $kk) {
			switch ($op[$k]['op_type']) {
			case XC_IS_UNUSED:
				$d[$kk] = '*UNUSED* ' . $op[$k]['u.opline_num'];
				break;

			case XC_IS_VAR:
				$d[$kk] = '$' . $op[$k]['u.var'];
				if ($kk != 'res') {
					$d[$kk] .= ':' . $this->getOpVal($op[$k], $EX);
				}
				break;

			case XC_IS_TMP_VAR:
				$d[$kk] = '#' . $op[$k]['u.var'];
				if ($kk != 'res') {
					$d[$kk] .= ':' . $this->getOpVal($op[$k], $EX);
				}
				break;

			case XC_IS_CV:
				$d[$kk] = $this->getOpVal($op[$k], $EX);
				break;

			default:
				if ($kk == 'res') {
					assert(0);
				}
				else {
					$d[$kk] = $this->getOpVal($op[$k], $EX);
				}
			}
		}
		$d['ext'] = $op['extended_value'];

		var_dump($d);
	}
	// }}}
	function dargs(&$EX, $indent) // {{{
	{
		$EX['indent'] = $indent;
		$op_array = &$EX['op_array'];

		if (isset($op_array['num_args'])) {
			$c = $op_array['num_args'];
		}
		else if ($op_array['arg_types']) {
			$c = count($op_array['arg_types']);
		}
		else {
			// php4
			$c = count($EX['recvs']);
		}

		$refrest = false;
		for ($i = 0; $i < $c; $i ++) {
			if ($i) {
				echo ', ';
			}
			if (isset($op_array['arg_info'])) {
				$ai = $op_array['arg_info'][$i];
				if (!empty($ai['class_name'])) {
					echo $ai['class_name'], ' ';
					if ($ai['allow_null']) {
						echo 'or NULL ';
					}
				}
				else if (!empty($ai['array_type_hint'])) {
					echo 'array ';
					if ($ai['allow_null']) {
						echo 'or NULL ';
					}
				}
				if ($ai['pass_by_reference']) {
					echo '&';
				}
				printf("\$%s", $ai['name']);
			}
			else {
				if ($refrest) {
					echo '&';
				}
				else if (isset($op_array['arg_types'][$i])) {
					switch ($op_array['arg_types'][$i]) {
					case BYREF_FORCE_REST:
						$refrest = true;
						/* fall */
					case BYREF_FORCE:
						echo '&';
						break;

					case BYREF_NONE:
					case BYREF_ALLOW:
						break;
					default:
						assert(0);
					}
				}
				$arg = $EX['recvs'][$i + 1];
				echo str($arg[0]);
				if (isset($arg[1])) {
					echo ' = ', str($arg[1]);
				}
			}
		}
	}
	// }}}
	function dfunction($func, $indent = '', $nobody = false) // {{{
	{
		if ($nobody) {
			$body = ";\n";
			$EX = array();
			$EX['op_array'] = &$func['op_array'];
			$EX['recvs'] = array();
		}
		else {
			ob_start();
			$newindent = INDENT . $indent;
			$EX = &$this->dop_array($func['op_array'], $newindent);
			$body = ob_get_clean();
			if (!isset($EX['recvs'])) {
				$EX['recvs'] = array();
			}
		}

		echo 'function ', $func['op_array']['function_name'], '(';
		$this->dargs($EX, $indent);
		echo ")\n";
		echo $indent, "{\n";
		echo $body;
		echo "$indent}\n";
	}
	// }}}
	function dclass($class, $indent = '') // {{{
	{
		// {{{ class decl
		if (!empty($class['doc_comment'])) {
			echo $indent;
			echo $class['doc_comment'];
			echo "\n";
		}
		$isinterface = false;
		if (!empty($class['ce_flags'])) {
			if ($class['ce_flags'] & ZEND_ACC_INTERFACE) {
				echo 'interface ';
				$isinterface = true;
			}
			else {
				if ($class['ce_flags'] & ZEND_ACC_IMPLICIT_ABSTRACT) {
					echo "abstract ";
				}
				if ($class['ce_flags'] & ZEND_ACC_FINAL) {
					echo "final ";
				}
			}
		}
		echo 'class ', $class['name'];
		if ($class['parent']) {
			echo ' extends ', $class['parent'];
		}
		/* TODO */
		if (!empty($class['interfaces'])) {
			echo ' implements ';
			echo implode(', ', $class['interfaces']);
		}
		echo "\n";
		echo $indent, "{";
		// }}}
		$newindent = INDENT . $indent;
		// {{{ const, static
		foreach (array('constants_table' => 'const '
					, 'static_members' => 'static $') as $type => $prefix) {
			if (!empty($class[$type])) {
				echo "\n";
				// TODO: skip shadow?
				foreach ($class[$type] as $name => $v) {
					echo $newindent;
					echo $prefix, $name, ' = ';
					echo str(value($v), $EX);
					echo ";\n";
				}
			}
		}
		// }}}
		// {{{ properties
		if (!empty($class['default_properties'])) {
			echo "\n";
			$infos = empty($class['properties_info']) ? null : $class['properties_info'];
			foreach ($class['default_properties'] as $name => $v) {
				$info = (isset($infos) && isset($infos[$name])) ? $infos[$name] : null;
				if (isset($info)) {
					if (!empty($info['doc_comment'])) {
						echo $newindent;
						echo $info['doc_comment'];
						echo "\n";
					}
				}

				echo $newindent;
				if (PHP_VERSION < 5) {
					echo 'var ';
				}
				else if (!isset($info)) {
					echo 'public ';
				}
				else {
					if ($info['flags'] & ZEND_ACC_SHADOW) {
						continue;
					}
					switch ($info['flags'] & ZEND_ACC_PPP_MASK) {
					case ZEND_ACC_PUBLIC:
						echo "public ";
						break;
					case ZEND_ACC_PRIVATE:
						echo "private ";
						break;
					case ZEND_ACC_PROTECTED:
						echo "protected ";
						break;
					}
					if ($info['flags'] & ZEND_ACC_STATIC) {
						echo "static ";
					}
				}

				echo '$', $name;
				if (isset($v)) {
					echo ' = ';
					echo str(value($v));
				}
				echo ";\n";
			}
		}
		// }}}
		// {{{ function_table
		if (isset($class['function_table'])) {
			foreach ($class['function_table'] as $func) {
				if (!isset($func['scope']) || $func['scope'] == $class['name']) {
					// TODO: skip shadow here
					echo "\n";
					$opa = $func['op_array'];
					if (!empty($opa['doc_comment'])) {
						echo $newindent;
						echo $opa['doc_comment'];
						echo "\n";
					}
					echo $newindent;
					if (isset($opa['fn_flags'])) {
						if ($opa['fn_flags'] & ZEND_ACC_ABSTRACT) {
							echo "abstract ";
						}
						if ($opa['fn_flags'] & ZEND_ACC_FINAL) {
							echo "final ";
						}
						if ($opa['fn_flags'] & ZEND_ACC_STATIC) {
							echo "static ";
						}

						switch ($opa['fn_flags'] & ZEND_ACC_PPP_MASK) {
							case ZEND_ACC_PUBLIC:
								echo "public ";
								break;
							case ZEND_ACC_PRIVATE:
								echo "private ";
								break;
							case ZEND_ACC_PROTECTED:
								echo "protected ";
								break;
							default:
								echo "<visibility error> ";
								break;
						}
					}
					$this->dfunction($func, $newindent, $isinterface);
					if ($opa['function_name'] == 'Decompiler') {
						//exit;
					}
				}
			}
		}
		// }}}
		echo $indent, "}\n";
	}
	// }}}
	function decompileString($string) // {{{
	{
		$this->dc = xcache_dasm_string($string);
		if ($this->dc === false) {
			echo "error compling string\n";
			return false;
		}
	}
	// }}}
	function decompileFile($file) // {{{
	{
		$this->dc = xcache_dasm_file($file);
		if ($this->dc === false) {
			echo "error compling $file\n";
			return false;
		}
	}
	// }}}
	function output() // {{{
	{
		echo "<?". "php\n";
		foreach ($this->dc['class_table'] as $key => $class) {
			if ($key{0} != "\0") {
				echo "\n";
				$this->dclass($class);
			}
		}

		foreach ($this->dc['function_table'] as $key => $func) {
			if ($key{0} != "\0") {
				echo "\n";
				$this->dfunction($func);
			}
		}

		echo "\n";
		$this->dop_array($this->dc['op_array']);
		echo "\n?" . ">\n";
		return true;
	}
	// }}}
}

// {{{ defines
define('ZEND_ACC_STATIC',         0x01);
define('ZEND_ACC_ABSTRACT',       0x02);
define('ZEND_ACC_FINAL',          0x04);
define('ZEND_ACC_IMPLEMENTED_ABSTRACT',       0x08);

define('ZEND_ACC_IMPLICIT_ABSTRACT_CLASS',    0x10);
define('ZEND_ACC_EXPLICIT_ABSTRACT_CLASS',    0x20);
define('ZEND_ACC_FINAL_CLASS',                0x40);
define('ZEND_ACC_INTERFACE',                  0x80);
define('ZEND_ACC_PUBLIC',     0x100);
define('ZEND_ACC_PROTECTED',  0x200);
define('ZEND_ACC_PRIVATE',    0x400);
define('ZEND_ACC_PPP_MASK',  (ZEND_ACC_PUBLIC | ZEND_ACC_PROTECTED | ZEND_ACC_PRIVATE));

define('ZEND_ACC_CHANGED',    0x800);
define('ZEND_ACC_IMPLICIT_PUBLIC',    0x1000);

define('ZEND_ACC_CTOR',       0x2000);
define('ZEND_ACC_DTOR',       0x4000);
define('ZEND_ACC_CLONE',      0x8000);

define('ZEND_ACC_ALLOW_STATIC',   0x10000);

define('ZEND_ACC_SHADOW', 0x2000);

define('ZEND_FETCH_GLOBAL',           0);
define('ZEND_FETCH_LOCAL',            1);
define('ZEND_FETCH_STATIC',           2);
define('ZEND_FETCH_STATIC_MEMBER',    3);
define('ZEND_FETCH_GLOBAL_LOCK',      4);

define('ZEND_FETCH_CLASS_DEFAULT',    0);
define('ZEND_FETCH_CLASS_SELF',       1);
define('ZEND_FETCH_CLASS_PARENT',     2);
define('ZEND_FETCH_CLASS_MAIN',       3);
define('ZEND_FETCH_CLASS_GLOBAL',     4);
define('ZEND_FETCH_CLASS_AUTO',       5);
define('ZEND_FETCH_CLASS_INTERFACE',  6);

define('ZEND_EVAL',               (1<<0));
define('ZEND_INCLUDE',            (1<<1));
define('ZEND_INCLUDE_ONCE',       (1<<2));
define('ZEND_REQUIRE',            (1<<3));
define('ZEND_REQUIRE_ONCE',       (1<<4));

define('ZEND_ISSET',              (1<<0));
define('ZEND_ISEMPTY',            (1<<1));
define('EXT_TYPE_UNUSED',         (1<<0));

define('ZEND_FETCH_STANDARD',     0);
define('ZEND_FETCH_ADD_LOCK',     1);

define('ZEND_FE_FETCH_BYREF',     1);
define('ZEND_FE_FETCH_WITH_KEY',  2);

define('ZEND_MEMBER_FUNC_CALL',   1<<0);
define('ZEND_CTOR_CALL',          1<<1);

define('ZEND_ARG_SEND_BY_REF',        (1<<0));
define('ZEND_ARG_COMPILE_TIME_BOUND', (1<<1));
define('ZEND_ARG_SEND_FUNCTION',      (1<<2));

define('BYREF_NONE',       0);
define('BYREF_FORCE',      1);
define('BYREF_ALLOW',      2);
define('BYREF_FORCE_REST', 3);
define('IS_NULL',     0);
define('IS_LONG',     1);
define('IS_DOUBLE',   2);
define('IS_STRING',   3);
define('IS_ARRAY',    4);
define('IS_OBJECT',   5);
define('IS_BOOL',     6);
define('IS_RESOURCE', 7);
define('IS_CONSTANT', 8);
define('IS_CONSTANT_ARRAY',   9);

@define('XC_IS_CV', 16);

/*
if (preg_match_all('!XC_[A-Z_]+!', file_get_contents(__FILE__), $ms)) {
	$verdiff = array();
	foreach ($ms[0] as $k) {
		if (!defined($k)) {
			$verdiff[$k] = -1;
			define($k, -1);
		}
	}
	var_export($verdiff);
}
/*/
foreach (array (
	'XC_HANDLE_EXCEPTION' => -1,
	'XC_FETCH_CLASS' => -1,
	'XC_FETCH_' => -1,
	'XC_FETCH_DIM_' => -1,
	'XC_ASSIGN_DIM' => -1,
	'XC_UNSET_DIM' => -1,
	'XC_FETCH_OBJ_' => -1,
	'XC_ASSIGN_OBJ' => -1,
	'XC_ISSET_ISEMPTY_DIM_OBJ' => -1,
	'XC_ISSET_ISEMPTY_PROP_OBJ' => -1,
	'XC_ISSET_ISEMPTY_VAR' => -1,
	'XC_INIT_METHOD_CALL' => -1,
	'XC_VERIFY_ABSTRACT_CLASS' => -1,
	'XC_DECLARE_CLASS' => -1,
	'XC_DECLARE_INHERITED_CLASS' => -1,
	'XC_ADD_INTERFACE' => -1,
	'XC_POST_DEC_OBJ' => -1,
	'XC_POST_INC_OBJ' => -1,
	'XC_PRE_DEC_OBJ' => -1,
	'XC_PRE_INC_OBJ' => -1,
	'XC_UNSET_OBJ' => -1,
	'XC_JMP_NO_CTOR' => -1,
	'XC_FETCH_' => -1,
	'XC_FETCH_DIM_' => -1,
	'XC_UNSET_DIM_OBJ' => -1,
	'XC_FETCH_OBJ_' => -1,
	'XC_ISSET_ISEMPTY' => -1,
	'XC_INIT_FCALL_BY_FUNC' => -1,
	'XC_DO_FCALL_BY_FUNC' => -1,
	'XC_DECLARE_FUNCTION_OR_CLASS' => -1,
) as $k => $v) {
	if (!defined($k)) {
		define($k, $v);
	}
}

/* XC_UNDEF XC_OP_DATA
$content = file_get_contents(__FILE__);
for ($i = 0; $opname = xcache_get_opcode($i); $i ++) {
	if (!preg_match("/\\bXC_" . $opname . "\\b(?!')/", $content)) {
		echo "not done ", $opname, "\n";
	}
}
// */
// }}}

