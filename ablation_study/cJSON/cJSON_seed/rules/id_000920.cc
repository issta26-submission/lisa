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
//<ID> 920
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *str = NULL;
    cJSON *trueItem = NULL;
    cJSON *retrieved_flag = NULL;
    cJSON *retrieved_greeting = NULL;
    const char *greeting = NULL;
    cJSON_bool added_greet = 0;
    cJSON_bool added_flag = 0;
    cJSON_bool is_true = 0;
    char *buffer = NULL;
    size_t bufsize = 32;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    str = cJSON_CreateString("hello");
    trueItem = cJSON_CreateTrue();
    added_greet = cJSON_AddItemReferenceToObject(root, "greeting", str);
    added_flag = cJSON_AddItemReferenceToObject(root, "flag", trueItem);

    // step 3: Operate / Validate
    retrieved_flag = cJSON_GetObjectItemCaseSensitive(root, "flag");
    retrieved_greeting = cJSON_GetObjectItemCaseSensitive(root, "greeting");
    greeting = cJSON_GetStringValue(retrieved_greeting);
    is_true = cJSON_IsTrue(retrieved_flag);
    buffer = (char *)cJSON_malloc(bufsize);
    memset(buffer, 0, bufsize);
    buffer[0] = (char)('0' + (char)is_true);
    buffer[1] = greeting[0];

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}