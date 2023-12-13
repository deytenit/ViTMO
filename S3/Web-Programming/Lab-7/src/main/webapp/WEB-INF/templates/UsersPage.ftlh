<#-- @ftlvariable name="user" type="ru.itmo.wp.model.domain.User" -->
<#import "commons.ftlh" as c>

<@c.page>
    <div class="users datatable">
        <div class="caption">User</div>
        <table>
            <thead>
            <tr>
                <th>Id</th>
                <th>Login</th>
                <#if user?? && user.admin>
                    <th>Admin</th>
                </#if>
            </tr>
            </thead>
            <tbody>
            <template>
                <tr>
                    <td class="user-id"></td>
                    <td class="user-login"></td>
                    <#if user?? && user.admin>
                        <td class="user-admin"><button></button></td>
                    </#if>
                </tr>
            </template>
            <tr class="noData">
                <td colspan=3>
                    No data
                </td>
            </tr>
            </tbody>
        </table>
    </div>

    <script>
        $(function () {
            const $noData = $(".users .noData");
            $noData.find("td").text("Loading...");

            ajax({action: "findAll"}, (response) => {
                response.users.forEach((user) => {
                    $noData.hide();
                    const $tr = $($(".users template").prop("content")).clone();
                    $tr.find(".user-id").text(user.id);
                    $tr.find(".user-login").text(user.login);
                    $tr.find("tr").attr("data-userId", user.id);
                    <#if user?? && user.admin>
                        const $button = $tr.find(".user-admin button");
                        $button.text(user.admin ? "disable" : "enable")
                        $button.click(() => {
                            ajax(
                                {action: "updateUserAdmin", id: user.id, admin: $button.text() === "enable"},
                                (response) => {
                                    console.log(response.admin ? "disable" : "enable");
                                    $button.text(response.admin ? "disable" : "enable");
                                    notify(response.error ?? "User has been updated!");
                                }
                            );
                        });
                    </#if>
                    $(".users tbody").append($tr);
                });

                $(".users tr").click(function () {
                    ajax(
                        {action: "findUser", userId: $(this).attr("data-userId")},
                        (response) => notify(response.user.creationTime)
                    );
                });
            });
        });
    </script>
</@c.page>
