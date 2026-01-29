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
//<ID> 2044
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[32];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *str_item = NULL;
    cJSON *true_item = NULL;
    cJSON *bool_added_item = NULL;
    cJSON *got_item = NULL;
    char *heap_buf = NULL;
    const char *got_str = NULL;
    cJSON_bool added_greeting = 0;
    cJSON_bool added_explicit_true = 0;
    cJSON_bool is_invalid = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("hello");
    heap_buf = (char *)cJSON_malloc(8);
    heap_buf[0] = 't';
    heap_buf[1] = 'e';
    heap_buf[2] = 's';
    heap_buf[3] = 't';
    heap_buf[4] = '\0';
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    added_greeting = cJSON_AddItemToObject(root, "greeting", str_item);
    added_explicit_true = cJSON_AddItemToObject(root, "explicit_true", true_item);
    bool_added_item = cJSON_AddBoolToObject(root, "flag", 1);

    // step 4: Operate
    got_item = cJSON_GetObjectItem(root, "greeting");
    got_str = cJSON_GetStringValue(got_item);
    is_invalid = cJSON_IsInvalid(bool_added_item);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(bool_added_item != NULL);
    validation ^= (int)(heap_buf != NULL);
    validation ^= (int)(got_item != NULL);
    validation ^= (int)(got_str != NULL);
    validation ^= (int)(got_str[0] == 'h');
    validation ^= (int)(added_greeting != 0);
    validation ^= (int)(added_explicit_true != 0);
    validation ^= (int)(is_invalid == 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(heap_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}