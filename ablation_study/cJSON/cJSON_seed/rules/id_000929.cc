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
//<ID> 929
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *name_item = NULL;
    cJSON *true_item = NULL;
    cJSON *alias_item = NULL;
    const char *initial_name = "example_name";
    const char *retrieved_name = NULL;
    cJSON_bool is_true = 0;
    int flag_value = 0;
    char *buffer = NULL;
    size_t bufsize = 32;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString(initial_name);
    cJSON_AddItemToObject(root, "name", name_item);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);
    cJSON_AddItemReferenceToObject(root, "flag_alias", true_item);

    // step 3: Operate / Validate
    retrieved_name = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    is_true = cJSON_IsTrue(cJSON_GetObjectItem(root, "flag_alias"));
    flag_value = (int)is_true;
    buffer = (char *)cJSON_malloc(bufsize);
    memset(buffer, 0, bufsize);
    buffer[0] = (retrieved_name != NULL && retrieved_name[0] != '\0') ? retrieved_name[0] : ' ';
    buffer[1] = (char)('0' + (flag_value % 10));

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}