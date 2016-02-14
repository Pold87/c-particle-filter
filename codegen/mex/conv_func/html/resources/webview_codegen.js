/**
 * Copyright 2015 The MathWorks, Inc.
 */

function webviewToCodeInit() {
    "use strict";

    document.getElementById("rtw_webview").contentWindow.require([
        "dojo/dom-style",
        "dojo/topic",
        "dojo/window",
        "dijit/registry"
    ], function (domStyle, topic, window,
                 registry) {

        var app = registry.byId("slwebview"),
            styler = {
                traceableColor: "#00cfcf",
                untraceableColor: "#e8d152",
                getColor: function (isTraceable) {
                    return isTraceable ? this.traceableColor : this.untraceableColor;
                },
                getStyle: function (element, isTraceable) {
                    var color = this.getColor(isTraceable);
                    return "fill:" + color + "; fill-opacity:0.5; stroke:" + color + ";";
                }
            },
            lastNode,
            // highlight MATLAB function line
            highlightLine = function (element, line, color) {
                app.open(element).then(function () {
                    var node,
                        elementViewer;

                    // elementViewer api subject to change
                    elementViewer = app.getElementViewer();
                    if (elementViewer) {
                        if (lastNode) {
                            domStyle.set(lastNode, "background", "");
                        }
                        node = elementViewer.getLineNode(line);
                        if (node) {
                            domStyle.set(node, "background", color);
                            window.scrollIntoView(node);
                            lastNode = node;
                        }
                    }
                });
            };
        
        // Go from webview to code
        topic.subscribe("slwebview/select", function (obj, opts) {
            var sid,
                line,
                isTraceable;

            if (app.isElement(obj)) {
                app.unhighlightAll();

                // Get sid.  If matlab function element, get it's sid instead of the block
                sid = obj.backing.sid;
                line = (opts && opts.line) ? opts.line : -1;
                if (line >= 0) {
                    sid = obj.backing.obj_viewer.code_sid + ":" + line.toString();
                }

                // Highlight code
                isTraceable = top.rtwChangeSysCallback(sid);
                
                // Highlight element base on whether it is traceable back to code
                app.highlight(obj, styler.getStyle(obj, isTraceable));
                if (line >= 0) {
                    highlightLine(obj, line, styler.getColor(isTraceable));
                }
            }
        });

        // Go from code to webview
        top.codeToWebView = function (sid, parentSid) {
            var element = app.getElement(sid),
                isTraceable = true, // always traceable
                line,
                idx;

            if (!element) {
                // sid is the internal implement chart, use parent sid
                idx = sid.lastIndexOf(":"); // last part is the line number
                line = sid.substring(idx + 1);
                element = app.getElement(parentSid);
            }

            if (element) {
                app.open(element.diagram).then(function () {
                    app.select(element.diagram); // unselect
                    app.unhighlightAll();
                    app.moveToView(element);
                    
                    // Highlight element
                    app.highlight(element, styler.getStyle(element, isTraceable));
                    if (line >= 0) {
                        highlightLine(element, line, styler.getColor(isTraceable));
                    }
                });
            }
        };
    });
}
