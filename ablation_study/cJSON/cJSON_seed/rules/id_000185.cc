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
//<ID> 185
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(3.14159);
    cJSON *str_item = cJSON_CreateString("pi");
    const char *version = cJSON_Version();
    void *buf = cJSON_malloc(16);
    memset(buf, 0, 16);
    double extracted = 0.0;
    cJSON_bool label_is_string = 0;
    cJSON *got_num = NULL;
    cJSON *got_label = NULL;
    cJSON *double_item = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "number", num_item);
    cJSON_AddItemToObject(root, "label", str_item);

    // step 3: Operate / Validate
    got_num = cJSON_GetObjectItem(root, "number");
    extracted = cJSON_GetNumberValue(got_num);
    double_item = cJSON_CreateNumber(extracted * 2.0);
    cJSON_AddItemToObject(root, "double", double_item);
    got_label = cJSON_GetObjectItem(root, "label");
    label_is_string = cJSON_IsString(got_label);
    ((char *)buf)[0] = version[0];
    ((char *)buf)[1] = '\0';

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)label_is_string;
    (void)extracted;
    (void)got_num;
    (void)got_label;
    return 66;
}