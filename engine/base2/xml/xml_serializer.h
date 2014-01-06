// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//
//	参考CEGUI的部分代码
//

#ifndef BASE2_XML_SERIALIZER_H_
#define BASE2_XML_SERIALIZER_H_
#pragma once

#include <string>
#include <vector>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace base {

/*!
    \brief 
         Class used to create XML Document. 

    This class hides the complexity of formatting valid XML files. The
    class provides automatic substitution of entities, XML indenting
    in respect of the spaces. It does not contains any codes specific
    to CEGUI taking appart the base::std::string class. The following
    example show the code needed to exports parts of an XML document
    similar to what can be found in a layout.
    
    @code 
    #include <iostream>
    #include <xml_serializer.h> 
    
    int main() 
    {
       // Create an encoder that outputs its result on standard output 
       XMLSerializer xml(std::cout, 4);
       xml.openTag("Window")
          .attribute("Type", "TaharezLook/StaticText")
          .attribute("Name", "Test")
          .openTag("Property")
          .attribute("Name", "Text")
          .text("This is the static text to be displayed")
          .closeTag()
          .openTag("Window")
          .attribute("Name", "Button")
          .attribute("Type", "Vanilla/Button")
          .openTag("Property")
          .attribute("Name", "Text")
          .attribute("Value", "Push me")
          .closeTag()
          .closeTag()
          .closeTag();
          
       if (xml)
       {
           std::cout << "XML Exported successfully" << std::endl;
       }
       return 0;
    }
    @endcode
*/

class XMLSerializer {
public:
	XMLSerializer(std::ostream& out, size_t indentSpace = 4);
	virtual ~XMLSerializer(void);  

	XMLSerializer& openTag(const std::string& name);
	XMLSerializer& closeTag(void);
	XMLSerializer& attribute(const std::string& name, const std::string& value);
	XMLSerializer& text(const std::string& text);
	unsigned int getTagCount() const;

	operator bool () const {
		return false == d_error;
	}
	bool operator!() const {
		return false != d_error;
	}

protected:
private:
	bool d_error; //!< Store the status of the serializer 
	unsigned int d_tagCount; //!<Return the number of tag in the document 
	size_t d_depth; //!< Store the current depth for indentation purpose 
	size_t d_indentSpace; //!< Store the number of space use for indenting 
	bool d_needClose; //!< Store whether the next operation need to close the tag or not 
	bool d_lastIsText; //!< Store whether the last operation was a text node or not 
	std::ostream& d_stream; //!< A reference to the stream object use
	std::vector<std::string> d_tagStack; //!< Store the tag stack for correct closing of the tags. 

	void indentLine();
	static std::string convertEntityInText(const std::string& text);
	static std::string convertEntityInAttribute(const std::string& attributeValue);

	// Disabled operation 
	XMLSerializer(const XMLSerializer& obj);
	// Disabled operation 
	XMLSerializer& operator=(const XMLSerializer& obj);  
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif 
