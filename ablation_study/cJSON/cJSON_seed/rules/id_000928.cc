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
//<ID> 928
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *greeting = NULL;
    cJSON *flag = NULL;
    cJSON *retrieved_flag = NULL;
    const char *text = "hello_world";
    const char *text_value = NULL;
    cJSON_bool is_true = 0;
    char *buffer = NULL;
    int buflen = 32;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    greeting = cJSON_CreateString(text);
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_AddItemReferenceToObject(root, "greeting_alias", greeting);
    flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "status", flag);
    cJSON_AddItemReferenceToObject(root, "status_alias", flag);

    // step 3: Operate / Validate
    retrieved_flag = cJSON_GetObjectItem(root, "status_alias");
    is_true = cJSON_IsTrue(retrieved_flag);
    text_value = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));
    buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, buflen);
    buffer[0] = (char)('0' + (int)is_true);
    buffer[1] = '-';
    buffer[2] = (text_value != NULL) ? text_value[0] : 'x';
    (void)buffer;
    (void)text_value;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}