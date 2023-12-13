<template>
  <div>
    <p v-for="(par, idx) in paragraphs" :key="`${keyPrefix}-${idx}`">{{ par }}</p>
  </div>
</template>

<script>
export default {
  name: "ParagraphsBox",
  props: ["text", "truncate", "keyPrefix"],
  data() {
    return {
      paragraphs: (this.truncate ? this.text.substring(0, 300) : this.text).split("\n")
    };
  }
};
</script>

<style scoped></style>
