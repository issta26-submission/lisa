#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 673
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(128);
    memset(scratch, 0, 128);
    const char *id_value = "1234";
    size_t to_copy = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool equal = 0;
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("test");
    cJSON *root_copy = cJSON_CreateObject();
    cJSON *meta_copy = cJSON_CreateObject();

    // step 2: Setup / Configure
    added1 = cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddStringToObject(root, "status", "ok");
    added2 = cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "id", id_value);
    cJSON_AddStringToObject(root_copy, "name", "test");
    cJSON_AddStringToObject(root_copy, "status", "ok");
    cJSON_AddItemToObject(root_copy, "meta", meta_copy);
    cJSON_AddStringToObject(meta_copy, "id", id_value);

    // step 3: Operate / Validate
    equal = cJSON_Compare(root, root_copy, 1);
    to_copy = strlen(id_value);
    memcpy(scratch, id_value, to_copy);
    scratch[to_copy] = '\0';

    // step 4: Cleanup
    cJSON_Delete(root_copy);
    cJSON_Delete(root);
    cJSON_free(scratch);
    // API sequence test completed successfully
    return 66;
}