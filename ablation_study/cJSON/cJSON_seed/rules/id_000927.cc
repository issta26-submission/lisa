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
//<ID> 927
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
    cJSON *t = NULL;
    cJSON *retrieved_flag = NULL;
    const char *msg = "hello";
    char *buffer = NULL;
    cJSON_bool is_true = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    greeting = cJSON_CreateString(msg);
    cJSON_AddItemToObject(root, "greeting", greeting);
    t = cJSON_CreateTrue();
    cJSON_AddItemReferenceToObject(root, "flag", t);
    buffer = (char *)cJSON_malloc(16);
    memset(buffer, 0, 16);

    // step 3: Operate / Validate
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    is_true = cJSON_IsTrue(retrieved_flag);
    buffer[0] = (char)('0' + (int)is_true);
    buffer[1] = (char)msg[0];

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}