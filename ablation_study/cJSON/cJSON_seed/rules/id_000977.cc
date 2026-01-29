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
//<ID> 977
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"name\":\"device\",\"value\":3.14,\"features\":[\"a\",\"b\"],\"meta\":{\"id\":42}}";
    const char *parse_end = NULL;
    cJSON *root = NULL;
    cJSON *meta = NULL;
    cJSON *id_item = NULL;
    const char *name_val = NULL;
    double value_val = 0.0;
    int id_val = 0;
    const char *version = NULL;
    char *unformatted = NULL;
    char *prealloc = NULL;
    int prealloc_len = 128;
    char *scratch = NULL;
    size_t scratch_len = 32;

    // step 2: Setup / Configure (construct the object programmatically to avoid parse failures)
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "device");
    cJSON_AddNumberToObject(root, "value", 3.14);
    cJSON *features = cJSON_CreateStringArray((const char *const[]){"a", "b"}, 2);
    cJSON_AddItemToObject(root, "features", features);
    meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNumberToObject(meta, "id", 42);

    version = cJSON_Version();
    unformatted = cJSON_PrintUnformatted(root);
    prealloc = (char *)cJSON_malloc((size_t)prealloc_len);
    memset(prealloc, 0, (size_t)prealloc_len);
    cJSON_PrintPreallocated(root, prealloc, prealloc_len, 0);
    scratch = (char *)cJSON_malloc(scratch_len);
    memset(scratch, 0, scratch_len);

    // step 3: Operate / Validate
    name_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    value_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    meta = cJSON_GetObjectItem(root, "meta");
    id_item = cJSON_GetObjectItem(meta, "id");
    id_val = id_item->valueint;
    scratch[0] = name_val[0];
    scratch[1] = version[0];
    scratch[2] = (char)('0' + (id_val % 10));
    scratch[3] = prealloc[0];
    scratch[4] = unformatted[0];

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_free(prealloc);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}