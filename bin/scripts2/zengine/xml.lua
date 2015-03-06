-- Copyright (C) 2012 by wubenqi
-- Distributable under the terms of either the Apache License (Version 2.0) or 
-- the GNU Lesser General Public License, as specified in the COPYING file.
--
-- By: wubenqi<wubenqi@gmail.com>
--

------------------------------------------------------------------------------------------
if XmlUtil == nil then
  XmlUtil = { class_name = "XmlUtil" }
end

function XmlUtil:CreateInstance(o)
	o = o or { class_name = "XmlUtil" }
	setmetatable(o, {__index = self})
	return o
end

function XmlUtil:ElementStart(element, attrs)
  TRACE("XmlUtil:ElementStart()")
  return 0
end

function XmlUtil:ElementEnd(element)
  TRACE("XmlUtil:ElementEnd()")
  return 0
end

function XmlUtil:Text(text)
  TRACE("XmlUtil:Text()")
  return 0
end

------------------------------------------------------------------------------------------
function ElementStart(xml, element, attrs)
  return xml:ElementStart(element, attrs)
end

function ElementEnd(xml, element)
  return xml:ElementEnd(element)
end

function Text(xml, text)
  return xml:Text(text)
end
