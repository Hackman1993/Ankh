# Why

I need to parse html with cpp, and query it with CSS3 selectors. So I developed this library with Boost Spirit X3

# Progress

Currently I'm working on it, it's **NOT** ready for production yet.

# Usage

## Supporting Status

### 🆗 Selector Supported Selectors
    
    ID Selector, eg: #id
    Class Selector, eg: #class
    Element Selector, eg: div
    Attribute Selector with "*^$" support, eg: [id],[class^='begin']
    Combined Selector, eg: div,input
    Descendants selector, eg: div input
    Direct Descendants Selector, eg: div>input
    Sibling Selector, eg: div~input, div+input
    Most of Pseudo class Selector, eg: :nth-child(odd) :nth-last-child(2n+1) :checked

### Selector Not Supported 

    Target Pseudo-class Selector, eg: :target
    Pseudo-element Selector, eg: ::hover

## Difference
:last-child selector's behavior is a little bit different from the original, please view those example below:

    <html>
        <head>
            <style></style>
        </head>
        <body>
            <p id="test"></p>
        </body>
    </html>
If you execute document.querySelectAll(":last-child") in browser, you may get 4 results:

    html,style,body,p#test

But if you do same query you will only get 3 results below:

    style,body,p#test
That's because I need to consider user may input a html block but a full html, like:

    <div id="id1">
        blablablabla...
    </div>
    <div id="id2">
        blablablabla...
    </div>

In this case, I won't know it's a real last-child,so root level tags won't join the match.

# Important Things

**THIS LIBRARY IS NOT READY FOR PRODUCTION**

# Thanks To
  sehe from stackoverflow