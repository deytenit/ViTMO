<template>
  <div class="comments-box">
    <table v-if="user">
      <tbody>
        <tr>
          <td class="user">
            <img src="https://userpic.codeforces.org/no-title.jpg" />
            <span>{{ user.login }}</span>
          </td>
          <td class="form-box" @submit.prevent="onWriteComment">
            <form method="post" action="">
              <input type="hidden" name="action" value="comment" />
              <textarea id="text" v-model="text" autofocus name="text"></textarea>
              <div class="error">{{ error }}</div>
              <input type="submit" value="Comment" />
            </form>
          </td>
        </tr>
      </tbody>
    </table>
    <div v-for="comment in comments" :key="comment.id" class="comment">
      <table>
        <tbody>
          <tr>
            <td class="user">
              <img src="https://userpic.codeforces.org/no-title.jpg" />
              <span>{{ comment.user.login }}</span>
            </td>
            <td class="content">
              <div class="information">{{ comment.id }}</div>
              <br />
              <div>
                <ParagraphsView :text="comment.text" :truncate="false" />
              </div>
            </td>
          </tr>
        </tbody>
      </table>
    </div>
  </div>
</template>

<script>
import ParagraphsView from "../misc/ParagraphsView";

export default {
  name: "CommentsView",
  components: { ParagraphsView },
  props: ["postId", "comments", "user", "updateFn"],
  data: function () {
    return {
      text: "",
      error: ""
    };
  },
  beforeMount() {
    this.$root.$on("onWriteCommentValidationError", (error) => (this.error = error));
  },
  methods: {
    onWriteComment: function () {
      this.error = "";
      this.$root.$emit("onWriteComment", this.postId, this.text, this.updateFn);
      this.text = "";
    }
  }
};
</script>

<style scoped></style>
