<template>
  <div id="app">
    <HeaderView :user="user" />
    <MiddleView :posts="posts" :user="user" />
    <FooterView />
  </div>
</template>

<script>
import HeaderView from "./components/HeaderView";
import MiddleView from "./components/MiddleView";
import FooterView from "./components/FooterView";
import axios from "axios";

export default {
  name: "App",
  components: {
    FooterView,
    MiddleView,
    HeaderView
  },
  data: function () {
    return {
      user: null,
      posts: []
    };
  },
  beforeMount() {
    if (localStorage.getItem("jwt") && !this.user) {
      this.$root.$emit("onJwt", localStorage.getItem("jwt"));
    }

    this.update();

    window.setInterval(() => {
      this.update();
    }, 5000);
  },
  beforeCreate() {
    this.$root.$on("onEnter", (login, password) => {
      if (password === "") {
        this.$root.$emit("onEnterValidationError", "Password is required");
        return;
      }

      axios
        .post("/api/1/jwt", {
          login,
          password
        })
        .then((response) => {
          localStorage.setItem("jwt", response.data);
          this.$root.$emit("onJwt", response.data);
        })
        .catch((error) => {
          this.$root.$emit("onEnterValidationError", error.response.data);
        });
    });

    this.$root.$on("onJwt", (jwt) => {
      localStorage.setItem("jwt", jwt);

      axios
        .get("/api/1/users/auth", {
          params: {
            jwt
          }
        })
        .then((response) => {
          this.user = response.data;
          this.$root.$emit("onChangePage", "Index");
        })
        .catch(() => this.$root.$emit("onLogout"));
    });

    this.$root.$on("onLogout", () => {
      localStorage.removeItem("jwt");
      this.user = null;
    });

    this.$root.$on("onWrite", (form) => {
      const jwt = localStorage.getItem("jwt");

      if (!this.user || !jwt) {
        this.$root.$emit("onChangePage", "Index");
      }

      let emited = false;
      const validate = (expr, msg) => {
        if (!emited && expr) {
          emited = true;
          this.$root.$emit("onWriteValidationError", msg);
        }
      };

      validate(!form.text || form.text.trim() === "", "Text cannot be blank");
      validate(form.text.length > 10000, "Text cannot be longer then 10000 characters");

      validate(!form.title || form.title.trim() === "", "Title cannot be blank");
      validate(form.title.length > 100, "Title cannot be longer then 100 characters");

      if (emited) {
        return;
      }

      axios
        .post("/api/1/posts", form, {
          params: {
            jwt
          }
        })
        .then(() => {
          this.$root.$emit("onChangePage", "Index");
        })
        .catch((error) => {
          this.$root.$emit("onWriteValidationError", error.response.data);
        });
    });

    this.$root.$on("onRegister", (form) => {
      if (this.user) {
        this.$root.$emit("onChangePage", "Index");
      }

      let emited = false;
      const validate = (expr, msg) => {
        if (!emited && expr) {
          emited = true;
          this.$root.$emit("onRegisterValidationError", msg);
        }
      };

      validate(!form.login || form.login.length < 3, "Login is too short");
      validate(form.login.length > 16, "Login is too long");
      validate(!/^[a-z]+$/.test(form.login), "Login can only contain lowercase latin characters");

      validate(!form.name || form.name.length < 1, "Name is too short");
      validate(form.name.length > 32, "Name is too long");

      validate(!form.password || form.password === "", "Password cannot be empty");

      if (emited) {
        return;
      }

      axios
        .post("/api/1/users", form)
        .then(() => {
          this.$root.$emit("onEnter", form.login, form.password);
        })
        .catch((error) => {
          this.$root.$emit("onRegisterValidationError", error.response.data);
        });
    });

    this.$root.$on("onWriteComment", (postId, text, callback) => {
      const jwt = localStorage.getItem("jwt");

      if (!this.user || !jwt || !postId) {
        this.$root.$emit("onChangePage", "Index");
      }

      let emited = false;
      const validate = (expr, msg) => {
        if (!emited && expr) {
          emited = true;
          this.$root.$emit("onWriteCommentValidationError", msg);
        }
      };

      validate(!text || text.trim() === "", "Text cannot be blank");
      validate(text.length > 10000, "Text cannot be longer then 10000 characters");

      if (emited) {
        return;
      }

      axios
        .post(
          `/api/1/post/${postId}/comments`,
          { text },
          {
            params: {
              jwt
            }
          }
        )
        .then(() => {
          callback();
        })
        .catch((error) => {
          this.$root.$emit("onWriteCommentValidationError", error.response.data);
        });
    });
  },
  methods: {
    update() {
      axios.get("/api/1/posts").then((response) => {
        this.posts = response.data;
      });
    }
  }
};
</script>

<style>
#app {
}
</style>
