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
//<ID> 980
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *n3 = NULL;
    cJSON *parsed = NULL;
    cJSON *vals = NULL;
    cJSON *first = NULL;
    cJSON *status_item = NULL;
    char mutable_json[] = "{\"name\":\"sensor\",\"vals\":[1,2,3],\"ok\":false}";
    char *printed = NULL;
    char *prebuf = NULL;
    size_t prebuf_len = 128;
    double first_val = 0.0;
    int status_is_false = 0;

    // step 2: Setup / Configure
    cJSON_Minify(mutable_json);
    parsed = cJSON_Parse(mutable_json);
    root = cJSON_CreateObject();
    data = cJSON_AddArrayToObject(root, "data");
    n1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(data, n1);
    n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(data, n2);
    n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(data, n3);
    cJSON_AddItemToObject(root, "status", cJSON_CreateFalse());
    printed = cJSON_PrintUnformatted(root);
    prebuf = (char *)cJSON_malloc(prebuf_len);
    memset(prebuf, 0, prebuf_len);
    cJSON_PrintPreallocated(root, prebuf, (int)prebuf_len, 1);

    // step 3: Operate / Validate
    vals = cJSON_GetObjectItem(parsed, "vals");
    first = cJSON_GetArrayItem(vals, 0);
    first_val = cJSON_GetNumberValue(first);
    status_item = cJSON_GetObjectItem(root, "status");
    status_is_false = (int)cJSON_IsFalse(status_item);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(prebuf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}