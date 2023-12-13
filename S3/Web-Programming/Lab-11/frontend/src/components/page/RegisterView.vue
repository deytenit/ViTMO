<template>
  <div class="form-box">
    <div class="header">Register</div>
    <div class="body">
      <form @submit.prevent="onRegister">
        <div class="field">
          <div class="name">
            <label for="title">Login:</label>
          </div>
          <div class="value">
            <input id="login" v-model="login" name="login" />
          </div>
        </div>
        <div class="field">
          <div class="name">
            <label for="name">Name:</label>
          </div>
          <div class="value">
            <input id="name" v-model="name" name="name" />
          </div>
        </div>
        <div class="field">
          <div class="name">
            <label for="password">Password:</label>
          </div>
          <div class="value">
            <input id="password" v-model="password" name="password" type="password" />
          </div>
        </div>
        <div class="error">{{ error }}</div>
        <div class="button-field">
          <input type="submit" value="Register" />
        </div>
      </form>
    </div>
  </div>
</template>

<script>
export default {
  name: "RegisterView",
  data: function () {
    return {
      login: "",
      name: "",
      password: "",
      error: ""
    };
  },
  beforeMount() {
    this.$root.$on("onRegisterValidationError", (error) => {
      this.error = error;
      this.password = "";
    });
  },
  methods: {
    onRegister: function () {
      this.error = "";
      this.$root.$emit("onRegister", {
        login: this.login,
        name: this.name,
        password: this.password
      });
    }
  }
};
</script>

<style scoped></style>
