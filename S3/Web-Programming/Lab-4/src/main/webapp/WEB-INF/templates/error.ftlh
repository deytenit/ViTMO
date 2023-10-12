<#-- @ftlvariable name="response_code" type="java.lang.Integer" -->
<#-- @ftlvariable name="error_msg" type="java.lang.String" -->
<#import "./common.ftlh" as c>

<@c.page>
    <div class="error-message-box">
        <h1>${response_code}</h1>
        <article>${error_msg}</article>
    </div>
</@c.page>
