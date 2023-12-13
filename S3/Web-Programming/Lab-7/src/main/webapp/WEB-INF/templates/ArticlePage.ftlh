<#import "commons.ftlh" as c>

<@c.page>
    <div class="form-box">
        <div class="header">Talk to Others</div>
        <form method="post" action="">
            <input type="hidden" name="action" value="talk"/>
            <div class="field">
                <div class="name">
                    <label for="title">Title</label>
                </div>
                <div class="value">
                    <input id="title" name="title">
                </div>
            </div>
            <div class="field">
                <div class="name">
                    <label for="text">Text</label>
                </div>
                <div class="value">
                    <textarea id="text" name="text" type="text"></textarea>
                </div>
            </div>
            <div class="field">
                <div class="name">
                    <label for="hidden">Is hidden?</label>
                </div>
                <div class="value">
                    <input id="hidden" name="hidden" type="checkbox">
                </div>
            </div>
            <div class="error"></div>
            <div class="button-field">
                <input type="submit" value="Create">
            </div>
        </form>
    </div>
    <script>
        $(function () {
            $(".form-box form").submit(function (event) {
                event.preventDefault();

                const title = $(this).find("input[name='title']").val();
                const text = $(this).find("textarea[name='text']").val();
                const hidden = $(this).find("input[name='hidden']").is(":checked");
                const $error = $(this).find(".error");

                console.log(hidden);

                ajax({
                    action: "create",
                    title,
                    text,
                    hidden
                }, (response) => {
                    $error.text(response.error);
                });
            });
        });
    </script>
</@c.page>
