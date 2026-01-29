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
//<ID> 72
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // This function demonstrates a typical sequence of cJSON API calls:
    // - create objects/arrays/values
    // - serialize to string
    // - parse back from string
    // - inspect and modify parsed JSON
    // - cleanup all allocations
    //
    // Returns 66 on success, non-66 on various failure points.

    cJSON *root = nullptr;
    cJSON *parsed = nullptr;
    char *printed = nullptr;
    char *printed2 = nullptr;

    // 1) Build a JSON object:
    // {
    //   "name": "John Doe",
    //   "age": 42,
    //   "married": false,
    //   "address": { "street": "123 Main St", "city": "Exampleville" },
    //   "phones": ["800-123-4567", "800-987-6543"]
    // }
    root = cJSON_CreateObject();
    if (!root) goto error;

    if (!cJSON_AddStringToObject(root, "name", "John Doe")) goto error;
    if (!cJSON_AddNumberToObject(root, "age", 42)) goto error;
    // Add boolean value
    cJSON_AddItemToObject(root, "married", cJSON_CreateFalse());

    // nested object: address
    {
        cJSON *address = cJSON_CreateObject();
        if (!address) goto error;
        if (!cJSON_AddStringToObject(address, "street", "123 Main St")) {
            cJSON_Delete(address);
            goto error;
        }
        if (!cJSON_AddStringToObject(address, "city", "Exampleville")) {
            cJSON_Delete(address);
            goto error;
        }
        cJSON_AddItemToObject(root, "address", address); // ownership transferred
    }

    // array: phones
    {
        cJSON *phones = cJSON_CreateArray();
        if (!phones) goto error;
        cJSON_AddItemToArray(phones, cJSON_CreateString("800-123-4567"));
        cJSON_AddItemToArray(phones, cJSON_CreateString("800-987-6543"));
        cJSON_AddItemToObject(root, "phones", phones); // ownership transferred
    }

    // 2) Serialize to compact JSON string
    printed = cJSON_PrintUnformatted(root);
    if (!printed) goto error;

    // 3) Parse the JSON string back to a cJSON structure
    parsed = cJSON_Parse(printed);
    if (!parsed) goto error;

    // 4) Inspect parsed JSON values and validate types
    {
        cJSON *name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
        if (!cJSON_IsString(name) || (name->valuestring == nullptr)) goto error;

        cJSON *age = cJSON_GetObjectItemCaseSensitive(parsed, "age");
        if (!cJSON_IsNumber(age)) goto error;

        cJSON *addr = cJSON_GetObjectItemCaseSensitive(parsed, "address");
        if (!cJSON_IsObject(addr)) goto error;
        cJSON *city = cJSON_GetObjectItemCaseSensitive(addr, "city");
        if (!cJSON_IsString(city)) goto error;

        cJSON *phones = cJSON_GetObjectItemCaseSensitive(parsed, "phones");
        if (!cJSON_IsArray(phones)) goto error;
        int phone_count = cJSON_GetArraySize(phones);
        if (phone_count != 2) goto error;
        cJSON *first_phone = cJSON_GetArrayItem(phones, 0);
        if (!cJSON_IsString(first_phone)) goto error;
    }

    // 5) Modify the parsed JSON: add a boolean "verified": true
    cJSON_AddItemToObject(parsed, "verified", cJSON_CreateTrue());

    // 6) Remove the second phone entry from the array
    {
        cJSON *phones = cJSON_GetObjectItemCaseSensitive(parsed, "phones");
        if (!cJSON_IsArray(phones)) goto error;
        // Delete item at index 1 (second element)
        cJSON_DeleteItemFromArray(phones, 1);
    }

    // 7) Re-serialize the modified parsed object (optional)
    printed2 = cJSON_PrintUnformatted(parsed);
    // printed2 may be null on allocation failure; we don't consider it fatal here,
    // but if allocated we must free it below.
    if (printed2) {
        free(printed2);
        printed2 = nullptr;
    }

    // 8) Cleanup all allocations and return success
    cJSON_Delete(parsed);
    parsed = nullptr;
    free(printed);
    printed = nullptr;
    cJSON_Delete(root);
    root = nullptr;

    return 66;

error:
    // On any failure, free what we've allocated so far.
    if (parsed) {
        cJSON_Delete(parsed);
        parsed = nullptr;
    }
    if (printed) {
        free(printed);
        printed = nullptr;
    }
    if (printed2) {
        free(printed2);
        printed2 = nullptr;
    }
    if (root) {
        cJSON_Delete(root);
        root = nullptr;
    }

    return 1;
}