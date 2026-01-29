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
//<ID> 985
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *msg_key = "message";
    const char *num_key = "value";
    const char *msg_text = "hello cJSON";
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *fetched_msg = (cJSON *)0;
    cJSON *fetched_num_cs = (cJSON *)0;
    char *json_str = (char *)0;
    const char *retrieved_msg = (const char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object and items
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString(msg_text);

    // step 3: Configure - attach string via AddItemToObject and add a number via AddNumberToObject
    cJSON_AddItemToObject(root, msg_key, str_item);
    num_item = cJSON_AddNumberToObject(root, num_key, 123.0);

    // step 4: Operate - serialize and retrieve items using both GetObjectItem variants
    json_str = cJSON_Print(root);
    fetched_msg = cJSON_GetObjectItem(root, msg_key);
    fetched_num_cs = cJSON_GetObjectItemCaseSensitive(root, num_key);
    retrieved_msg = cJSON_GetStringValue(fetched_msg);

    // step 5: Validate - compute a simple validation score from the retrieved pointers and values
    validation_score = (int)(root != (cJSON *)0) + (int)(fetched_msg == str_item) + (int)(fetched_num_cs == num_item) + (int)(retrieved_msg != (const char *)0 && retrieved_msg[0] == 'h');
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete root (which deletes attached items)
    cJSON_free(json_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}