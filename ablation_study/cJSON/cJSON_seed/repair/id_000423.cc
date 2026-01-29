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
//<ID> 423
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *title = cJSON_CreateString("demo");

    // step 2: Configure
    cJSON_AddNumberToObject(meta, "version", 1.5);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "title", title);
    cJSON *author = cJSON_CreateString("tester");
    cJSON_AddItemToObject(root, "author", author);

    // step 3: Operate
    cJSON *meta_ref = cJSON_GetObjectItem(root, "meta");
    cJSON *ver_item = cJSON_GetObjectItem(meta_ref, "version");
    double version = cJSON_GetNumberValue(ver_item);
    cJSON_bool is_obj = cJSON_IsObject(meta_ref);
    double computed = version * 2.0 + (double)is_obj;
    cJSON_AddNumberToObject(root, "computed", computed);

    // step 4: Validate and Cleanup
    cJSON *comp_item = cJSON_GetObjectItem(root, "computed");
    double retrieved = cJSON_GetNumberValue(comp_item);
    (void)retrieved;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}