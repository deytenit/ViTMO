<template>
  <Datatable :keys="keys" :titles="titles" :data="data" />
</template>

<script>
import DatatableBox from "../datatable/DatatableBox.vue";

export default {
  name: "UsersPage",
  components: {
    Datatable: DatatableBox
  },
  props: ["users"],
  computed: {
    keys: function () {
      return ["id", "login", "name"];
    },
    titles: function () {
      return {
        id: "Id",
        login: "Login",
        name: "Name"
      };
    },
    data: function () {
      return Object.values(this.users).sort((a, b) => a.id - b.id);
    }
  }
};
</script>

<style scoped></style>
