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
//<ID> 1335
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *msg_item = NULL;
    cJSON *double_item = NULL;
    double value = 0.0;
    const char *msg_str = NULL;
    char *out = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "num", cJSON_CreateNumber(123.25));
    cJSON_AddItemToObject(root, "msg", cJSON_CreateString("hello_cJSON"));

    // step 3: Operate / Validate
    num_item = cJSON_GetObjectItem(root, "num");
    value = cJSON_GetNumberValue(num_item);
    double_item = cJSON_CreateNumber(value * 2.0);
    cJSON_AddItemToObject(root, "num_doubled", double_item);
    msg_item = cJSON_GetObjectItem(root, "msg");
    msg_str = cJSON_GetStringValue(msg_item);
    out = cJSON_PrintUnformatted(root);
    (void)msg_str;
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}