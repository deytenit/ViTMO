<template>
  <div class="form">
    <div class="header">Write Post</div>
    <div class="body">
      <form @submit.prevent="onWritePost">
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
  name: "WritePostPage",
  data: function () {
    return {
      title: "",
      text: "",
      error: ""
    };
  },
  beforeMount() {
    this.$root.$on("onWritePostValidationError", (error) => (this.error = error));
  },
  methods: {
    onWritePost: function () {
      this.error = "";
      this.$root.$emit("onWritePost", this.title, this.text);
    }
  }
};
</script>

<style scoped></style>
