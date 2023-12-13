<#import "commons.ftlh" as c>

<@c.page>
    <div class="form-box">
        <div class="header">Enter</div>
        <div class="body">
            <form method="post" action="">
                <div class="field">
                    <div class="name">
                        <label for="loginOrEmail">Login/Email</label>
                    </div>
                    <div class="value">
                        <input id="loginOrEmail" name="loginOrEmail"/>
                    </div>
                </div>
                <div class="field">
                    <div class="name">
                        <label for="password">Password</label>
                    </div>
                    <div class="value">
                        <input id="password" name="password" type="password"/>
                    </div>
                </div>
                <div class="error"></div>
                <div class="button-field">
                    <input type="submit" value="Enter">
                </div>
            </form>
        </div>
    </div>
    <script>
        $(function () {
            $(".form-box form").submit(function (event) {
                event.preventDefault();

                const loginOrEmail = $(this).find("input[name='loginOrEmail']").val();
                const $password = $(this).find("input[name='password']");
                const $error = $(this).find(".error");

                ajax({
                    action: "enter",
                    loginOrEmail,
                    password: $password.val()
                }, (response) => {
                    $password.val("");
                    $error.text(response.error);
                });
            });
        });
    </script>
</@c.page>
