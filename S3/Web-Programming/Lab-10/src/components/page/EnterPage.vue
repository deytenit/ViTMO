<template>
  <div class="enter form-box">
    <div class="header">Enter</div>
    <div class="body">
      <form @submit.prevent="onEnter">
        <div class="field">
          <div class="name">
            <label for="login">Login</label>
          </div>
          <div class="value">
            <input id="login" v-model="login" autofocus name="login" />
          </div>
        </div>
        <div class="field">
          <div class="name">
            <label for="password">Password</label>
          </div>
          <div class="value">
            <input id="password" v-model="password" name="password" type="password" />
          </div>
        </div>
        <div class="field error">{{ error }}</div>
        <div class="button-field">
          <input type="submit" value="Enter" />
        </div>
      </form>
    </div>
  </div>
</template>

<script>
export default {
  name: "EnterPage",
  data: function () {
    return {
      login: "",
      password: "",
      error: ""
    };
  },
  beforeCreate() {
    this.$root.$on("onEnterValidationError", (error) => (this.error = error));
  },
  methods: {
    onEnter: function () {
      this.$root.$emit("onEnter", this.login, this.password);
    }
  }
};
</script>

<style scoped></style>
