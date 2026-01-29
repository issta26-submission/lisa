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
//<ID> 836
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *root_copy = NULL;
    cJSON *name = NULL;
    cJSON *id_num = NULL;
    cJSON *name2 = NULL;
    cJSON *id_num2 = NULL;
    char *printed = NULL;
    char buffer[64];
    cJSON_bool added_name = 0;
    cJSON_bool added_id = 0;
    cJSON_bool added_name2 = 0;
    cJSON_bool added_id2 = 0;
    cJSON_bool equal = 0;
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name = cJSON_CreateString("example");
    added_name = cJSON_AddItemToObject(root, "name", name);
    id_num = cJSON_CreateNumber(123.0);
    added_id = cJSON_AddItemToObject(root, "id", id_num);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    root_copy = cJSON_CreateObject();
    name2 = cJSON_CreateString("example");
    added_name2 = cJSON_AddItemToObject(root_copy, "name", name2);
    id_num2 = cJSON_CreateNumber(123.0);
    added_id2 = cJSON_AddItemToObject(root_copy, "id", id_num2);
    equal = cJSON_Compare(root, root_copy, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root_copy);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    (void)added_name;
    (void)added_id;
    (void)added_name2;
    (void)added_id2;
    (void)equal;
    return 66;
}