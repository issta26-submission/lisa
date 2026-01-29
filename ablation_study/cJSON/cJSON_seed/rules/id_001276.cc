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
//<ID> 1276
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *answer_node = NULL;
    cJSON *null_node = NULL;
    cJSON *retrieved_answer = NULL;
    cJSON *retrieved_status = NULL;
    const char *status_text = NULL;
    char *printed = NULL;

    root = cJSON_CreateObject();

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "pi", 3.1415);
    answer_node = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", answer_node);
    cJSON_AddStringToObject(root, "status", "ok");
    null_node = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "missing", null_node);

    // step 3: Operate / Validate
    retrieved_answer = cJSON_GetObjectItem(root, "answer");
    retrieved_status = cJSON_GetObjectItem(root, "status");
    status_text = cJSON_GetStringValue(retrieved_status);
    printed = cJSON_PrintUnformatted(root);
    (void)retrieved_answer;
    (void)status_text;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}