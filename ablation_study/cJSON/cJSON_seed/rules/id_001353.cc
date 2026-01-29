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
//<ID> 1353
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
    cJSON *true_item = NULL;
    char *printed = NULL;
    const char *version = NULL;
    double extracted_value = 0.0;

    // step 2: Setup / Configure
    version = cJSON_Version();
    root = cJSON_CreateObject();
    meta = cJSON_AddObjectToObject(root, "meta");
    num_item = cJSON_AddNumberToObject(meta, "value", 123.456);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "ok", true_item);
    cJSON_AddStringToObject(root, "version", version);

    // step 3: Operate / Validate
    extracted_value = cJSON_GetNumberValue(num_item);
    printed = cJSON_PrintUnformatted(root);
    (void)extracted_value;
    (void)version;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}