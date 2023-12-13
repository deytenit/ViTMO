<template>
  <DatatableView :keys="keys" :titles="titles" :data="data" />
</template>

<script>
import axios from "axios";
import DatatableView from "../datatable/DatatableView";

export default {
  name: "UsersView",
  components: {
    DatatableView
  },
  data() {
    return {
      keys: ["id", "login", "name"],
      titles: {
        id: "Id",
        login: "Login",
        name: "Name"
      },
      data: []
    };
  },
  beforeMount() {
    this.$root.$on("onUpdateData", (data) => {
      if (data === "users") {
        axios.get("/api/1/users").then((response) => {
          this.data = response.data.sort((a, b) => a.id - b.id);
        });
      }
    });

    this.$root.$emit("onUpdateData", "users");
  }
};
</script>

<style scoped></style>
