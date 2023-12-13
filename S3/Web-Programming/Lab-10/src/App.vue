<template>
  <div id="app">
    <Header :user-id="userId" :users="users" />
    <Middle :posts="posts" :users="users" :comments="comments" :user-id="userId" />
    <Footer :user-count="userCount" :post-count="postCount" :user-id="userId" />
  </div>
</template>

<script>
import HeaderBox from "./components/HeaderBox";
import MiddleBox from "./components/MiddleBox";
import FooterBox from "./components/FooterBox";

export default {
  name: "App",
  components: {
    Footer: FooterBox,
    Middle: MiddleBox,
    Header: HeaderBox
  },
  data: function () {
    return this.$root.$data;
  },
  computed: {
    userCount() {
      return Object.keys(this.$root.$data.users).length;
    },
    postCount() {
      return Object.keys(this.$root.$data.posts).length;
    }
  },
  beforeCreate() {
    this.$root.$on("onEnter", (login, password) => {
      if (password === "") {
        this.$root.$emit("onEnterValidationError", "Password is required");
        return;
      }

      const users = Object.values(this.users).filter((u) => u.login === login);
      if (users.length === 0) {
        this.$root.$emit("onEnterValidationError", "No such user");
      } else {
        this.userId = users[0].id;
        this.$root.$emit("onChangePage", "Index");
      }
    });

    this.$root.$on("onLogout", () => (this.userId = null));

    this.$root.$on("onRegister", (login, name) => {
      let emited = false;

      const error = (message) => {
        this.$root.$emit("onRegisterValidationError", message);
        emited = true;
      };

      if (this.userId) {
        error("Already In");
        return;
      }

      if (!login) {
        error("Login is required");
        return;
      }

      if (login.length < 3) {
        error("Login is too short");
      } else if (login.length > 16) {
        error("Login is too long");
      } else if (!/^[a-z]+$/.test(login)) {
        error("Login can only contain lowercase characters");
      } else if (Object.values(this.users).find((x) => x.login === login)) {
        error("Login already exists");
      }

      if (!name) {
        error("Name is required");
        return;
      }

      if (name.length < 1) {
        error("Name is too short");
      } else if (name.length > 32) {
        error("Name is too long");
      }

      if (emited) {
        return;
      }

      const id = Math.max(...Object.keys(this.users)) + 1;
      this.$root.$set(this.users, id, {
        id,
        login,
        name,
        admin: false
      });

      this.$root.$emit("onChangePage", "Enter");
    });

    this.$root.$on("onWritePost", (title, text) => {
      if (this.userId) {
        if (!title || title.length < 5) {
          this.$root.$emit("onWritePostValidationError", "Title is too short");
        } else if (!text || text.length < 10) {
          this.$root.$emit("onWritePostValidationError", "Text is too short");
        } else {
          const id = Math.max(...Object.keys(this.posts)) + 1;
          this.$root.$set(this.posts, id, {
            id,
            title,
            text,
            userId: this.userId
          });
        }
      } else {
        this.$root.$emit("onWritePostValidationError", "No access");
      }
    });

    this.$root.$on("onEditPost", (id, text) => {
      if (this.userId) {
        if (!id) {
          this.$root.$emit("onEditPostValidationError", "ID is invalid");
        } else if (!text || text.length < 10) {
          this.$root.$emit("onEditPostValidationError", "Text is too short");
        } else {
          let posts = Object.values(this.posts).filter((p) => p.id === parseInt(id));
          if (posts.length) {
            posts.forEach((item) => {
              item.text = text;
            });
          } else {
            this.$root.$emit("onEditPostValidationError", "No such post");
          }
        }
      } else {
        this.$root.$emit("onEditPostValidationError", "No access");
      }
    });

    this.$root.$on("onWriteComment", (postId, text) => {
      if (!this.userId) {
        this.$root.$emit("onWriteCommentValidationError", "No access");
        return;
      }

      if (!text) {
        this.$root.$emit("onWriteCommentValidationError", "Text is empty");
        return;
      }

      if (text.length < 1) {
        this.$root.$emit("onWriteCommentValidationError", "Text is too short");
      } else if (text.length > 64) {
        this.$root.$emit("onWriteCommentValidationError", "Text is too long");
      } else {
        const id = Math.max(...Object.keys(this.comments)) + 1;
        this.$root.$set(this.comments, id, {
          id,
          text,
          postId,
          userId: this.userId
        });
      }
    });
  }
};
</script>

<style>
#app {
}
</style>
