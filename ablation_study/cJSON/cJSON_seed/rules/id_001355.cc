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
//<ID> 1355
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *meta = NULL;
    cJSON *num_item = NULL;
    cJSON *inner_num = NULL;
    char *printed = NULL;
    const char *ver = NULL;
    double extracted = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_AddNumberToObject(root, "count", 42.0);
    cJSON_AddItemToObject(root, "enabled", cJSON_CreateTrue());
    meta = cJSON_AddObjectToObject(root, "meta");
    inner_num = cJSON_AddNumberToObject(meta, "inner", 3.14159);

    // step 3: Operate / Validate
    num_item = cJSON_GetObjectItem(root, "count");
    extracted = cJSON_GetNumberValue(num_item);
    ver = cJSON_Version();
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)inner_num;
    (void)extracted;
    (void)ver;
    return 66;
}