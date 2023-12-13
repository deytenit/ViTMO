<#-- @ftlvariable name="user" type="ru.itmo.wp.model.domain.User" -->
<#-- @ftlvariable name="talks" type="ru.itmo.wp.model.domain.Talk[]" -->
<#-- @ftlvariable name="logins" type="java.util.HashMap<java.lang.String, java.lang.String>" -->
<#-- @ftlvariable name="text" type="java.lang.String" -->
<#-- @ftlvariable name="error" type="java.lang.String" -->
<#import "commons.ftlh" as c>

<@c.page>
    <div class="form-box">
        <div class="header">Talk to Others</div>
        <form method="post" action="">
            <div class="field">
                <div class="name">
                    <label for="target">Recipient</label>
                </div>
                <select class="value" name="target" id="target">
                    <#list logins as key, value>
                        <#if key != user.id?string>
                            <option value="${value}">${value}</option>
                        </#if>
                    </#list>
                </select>
            </div>
            <div class="field">
                <div class="name">
                    <label for="text">Message</label>
                </div>
                <div class="value">
                    <textarea id="text" name="text" type="text">${text!}</textarea>
                </div>
            </div>
            <#if error??>
                <div class="error">
                    ${error!}
                </div>
            </#if>
            <div class="button-field">
                <input type="submit" value="Send">
            </div>
        </form>
    </div>
    <br/>
    <div class="datatable">
        <div class="caption">Talk</div>
        <table>
            <thead>
            <tr>
                <th>Id</th>
                <th>From</th>
                <th>To</th>
                <th>Text</th>
            </tr>
            </thead>
            <tbody>
            <#list talks as talk>
            <tr>
                <td>${talk.id}</td>
                <td>${logins[talk.sourceUserId?string]}</td>
                <td>${logins[talk.targetUserId?string]}</td>
                <td>${talk.text?truncate(32, "...")}</td>
            </tr>
            </#list>
            </tbody>
        </table>
    </div>
</@c.page>
