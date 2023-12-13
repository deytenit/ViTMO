<template>
  <div class="form-box">
    <div class="header">Write Post</div>
    <div class="body">
      <form @submit.prevent="onWrite">
        <div class="field">
          <div class="name">
            <label for="title">Title:</label>
          </div>
          <div class="value">
            <input id="title" v-model="title" name="title" />
          </div>
        </div>
        <div class="field">
          <div class="name">
            <label for="text">Text:</label>
          </div>
          <div class="value">
            <textarea id="text" v-model="text" name="text"></textarea>
          </div>
        </div>
        <div class="error">{{ error }}</div>
        <div class="button-field">
          <input type="submit" value="Write" />
        </div>
      </form>
    </div>
  </div>
</template>

<script>
export default {
  name: "WriteView",
  data: function () {
    return {
      title: "",
      text: "",
      error: ""
    };
  },
  beforeMount() {
    this.$root.$on("onWriteValidationError", (error) => (this.error = error));
  },
  methods: {
    onWrite: function () {
      this.error = "";
      this.$root.$emit("onWrite", { title: this.title, text: this.text });
    }
  }
};
</script>

<style scoped>
.form-box {
  width: 100%;
}

.form-box textarea {
  min-width: 30rem;
  max-width: 40rem;
}
</style>
