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
//<ID> 1230
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *payload = NULL;
    cJSON *num = NULL;
    cJSON *b = NULL;
    cJSON *list = NULL;
    cJSON *retrieved_payload = NULL;
    cJSON *retrieved_num = NULL;
    cJSON *retrieved_list = NULL;
    cJSON *first_in_list = NULL;
    char *printed = NULL;
    double new_number_value = 0.0;
    cJSON_bool was_false = 0;
    int list_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    payload = cJSON_CreateObject();
    num = cJSON_CreateNumber(3.14159);
    b = cJSON_CreateFalse();
    list = cJSON_CreateArray();
    cJSON_AddItemToObject(payload, "n", num);
    cJSON_AddItemReferenceToArray(list, num);
    cJSON_AddItemToObject(payload, "list", list);
    cJSON_AddItemToObject(payload, "flag", b);
    cJSON_AddItemToObject(root, "payload", payload);

    // step 3: Operate / Validate
    retrieved_payload = cJSON_GetObjectItem(root, "payload");
    retrieved_num = cJSON_GetObjectItem(retrieved_payload, "n");
    new_number_value = cJSON_SetNumberHelper(retrieved_num, 100.5);
    retrieved_list = cJSON_GetObjectItem(retrieved_payload, "list");
    list_size = cJSON_GetArraySize(retrieved_list);
    first_in_list = cJSON_GetArrayItem(retrieved_list, 0);
    was_false = cJSON_IsFalse(cJSON_GetObjectItem(retrieved_payload, "flag"));
    (void)first_in_list;
    (void)list_size;
    (void)new_number_value;
    (void)was_false;

    // step 4: Cleanup
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}